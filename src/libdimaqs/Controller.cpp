/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "Controller.h"
#include "dmaqs_plugin.h"
#include "mysql/service_my_plugin_log.h"
#include "AbstractClassifier.h"

#include "my_global.h"
#include "mysql/plugin_audit.h"
#include "sql_parse.h"


#include "DatabasePetriNetQuery.h"
#include "PetriNetClassifier.h"
#include "TransitionHelper.h"
#include "../../lib/petrinet/src/Place.h"
#include "../../lib/petrinet/src/Transition.h"
#include "DatabasePetriNet.h"

#include "transitionaction/AlwaysAction.h"
#include "transitionaction/CreateTableAction.h"
#include "transitionaction/DropDatabaseAction.h"
#include "transitionaction/DropTableAction.h"
#include "transitionaction/InsertAction.h"
#include "transitionaction/ListColumnAction.h"
#include "transitionaction/ListDatabaseAction.h"
#include "transitionaction/ListTableAction.h"
#include "transitionaction/ModifyTableAction.h"
#include "transitionaction/SetVariableAction.h"
#include "transitionaction/ShowVariableAction.h"

#include "placeaction/AdminModeAction.h"
#include "placeaction/BackupAction.h"
#include "placeaction/NotificationAction.h"
#include "placeaction/RewriteQueryAction.h"

#include "TriggerCreator.h"
#include "QueryRewriter.h"
#include "sstream"

#include "dmaqs_global.h"

#include "RelatedObjectTransitionCondition.h"

//static mysql_rwlock_t LOCK_dmaqs_classifier;
//PSI_rwlock_key key_rwlock_LOCK_dmaqs_classifier_;

#include "mutex"
using namespace std;

#include "string"
#include "vector"

#include "services.h"
#include "placeaction/CreateTriggerAction.h"

vector<string> split(const string &s, char delim) {
        vector<string> elems;
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }


Controller* Controller::s_instance = 0;

Controller::Controller()
    : triggersCreated_(false)
{
    s_instance = this;
    triggerCreator = new TriggerCreator(excluded_dbs_, object_prefix_);
    queryRewriter_ = new QueryRewriter(object_prefix_, storagespace_);
    adminHandler_ = new AdminHandler(secret_);
}


Controller::~Controller()
{
    while( classifier_.size() > 0){
        AbstractClassifier* obj = classifier_.back();
        classifier_.pop_back();
        delete obj;
    }

    delete triggerCreator;
    delete queryRewriter_;
    delete adminHandler_;
}

void Controller::init()
{

    PetriNetClassifier* pnc = new PetriNetClassifier();
    addClassifier(pnc);

    DatabasePetriNet* dpn = new DatabasePetriNet();
    pnc->addPetriNet(dpn);

    char const *rma[] = {"\\d*[.]{0,1}\\d*\\s*(BTC|BITCOIN)"};
    std::list<std::string> ransom_message_values(rma, rma + sizeof(rma) / sizeof(*rma));

    // this was not used for the tests
    char const *mtn[] = {
                        "WARNING",
                        "README_MISSING_DATABASES",
                        "PWNED",
                        "PWNED_SECURE_YOUR_STUFF_SILLY",
                        "ReadmePlease",
                        "CONTACTME",
                        "PLEASE_READ",
                        "WARNING_ALERT",
                        "to_get_DB_back_send_1BTC_to_1DGztzLNz1euFswtqMDWPMWSgwthdpxRtC",
                        "DELETED_BECAUSE_YOU_DIDNT_PASSWORD_PROTECT_YOUR_MONGODB",
                        "PLEASE_READ_56b41cc944bd390932e79827",
                        "README",
                        "LEIA_ME",
                        "AVISO_LEIA_ME",
                        "IHAVEYOURDATA",
                        "READ_ME",
                        "READMEPLS",
                        "ENCRYPTED",
                        "READ1",
                        "README_YOU_DB_IS_INSECURE",
                        "AVISO",
                        "DB_H4CK3D",
                        "PLEASEREAD",
                        "DB_DROPPED",
                        "REQUEST_YOUR_DATA",
                        "BACKUP_DB",
                        "Attention",
                        "PLEASE_READ_ME",
                        "PLEASREADTHIS"
    };
    std::list<std::string> malicious_table_names;//(mtn, mtn + sizeof(mtn) / sizeof(*mtn));

    petrinet::Place* pI1 = new petrinet::Place("I1");
    petrinet::Place* pI2 = new petrinet::Place("I2");
    petrinet::Place* pI3 = new petrinet::Place("I3");
    petrinet::Place* pI4 = new petrinet::Place("I4");
    petrinet::Place* pI5 = new petrinet::Place("I5");
    petrinet::Place* pOL1 = new petrinet::Place("OL1");
    petrinet::Place* pOL2 = new petrinet::Place("OL2");
    petrinet::Place* pOL3 = new petrinet::Place("OL3");
    petrinet::Place* pTC = new petrinet::Place("TC");
    petrinet::Place* pOD = new petrinet::Place("OD");
    petrinet::Place* pRI = new petrinet::Place("RI");
    petrinet::Place* pN = new petrinet::Place("N");
    petrinet::Place* pVA = new petrinet::Place("VA");
    petrinet::Place* pVS = new petrinet::Place("VS");

    pOD->addAction(new placeaction::BackupAction(*queryRewriter_));
    pN->addAction(new placeaction::NotificationAction(recipient_));
    pTC->addAction(new placeaction::CreateTriggerAction(triggerCreator));
    pVA->addAction(new placeaction::AdminModeAction(*adminHandler_));
    pOL1->addAction(new placeaction::RewriteQueryAction(*queryRewriter_, placeaction::RewriteQueryAction::Database));
    pVS->addAction(new placeaction::RewriteQueryAction(*queryRewriter_, placeaction::RewriteQueryAction::Variable));

    dpn->addPlace(pI1, true);
    dpn->addPlace(pI2, true);
    dpn->addPlace(pI3, true);
    dpn->addPlace(pI4, true);
    dpn->addPlace(pI5, true);

    dpn->addPlace(pOL1, false);
    dpn->addPlace(pOL2, false);
    dpn->addPlace(pOL3, false);
    dpn->addPlace(pTC, false);
    dpn->addPlace(pOD, false);
    dpn->addPlace(pRI, false);
    dpn->addPlace(pN, false);
    dpn->addPlace(pVA, false);
    dpn->addPlace(pVS, false);

    petrinet::Transition* tLD = new petrinet::Transition("LD");
    petrinet::Transition* tLT = new petrinet::Transition("LT");
    petrinet::Transition* tLC= new petrinet::Transition("LC");
    petrinet::Transition* tCT = new petrinet::Transition("CT");
    petrinet::Transition* tDD = new petrinet::Transition("DD");
    petrinet::Transition* tDT = new petrinet::Transition("DT");
    petrinet::Transition* tMT = new petrinet::Transition("MT");
    petrinet::Transition* tI = new petrinet::Transition("I");
    petrinet::Transition* tA = new petrinet::Transition("A");
    petrinet::Transition* tAV = new petrinet::Transition("AV");
    petrinet::Transition* tSV = new petrinet::Transition("SV");

    RelatedObjectTransitionCondition* tCT_tI_cond = new RelatedObjectTransitionCondition(TableName);
    tCT_tI_cond->setTransition(tCT);
    tI->addCondition(tCT_tI_cond);

    tLD->setAction(new transitionaction::ListDatabaseAction());
    tLT->setAction(new transitionaction::ListTableAction());
    tLC->setAction(new transitionaction::ListColumnAction());
    tCT->setAction(new transitionaction::CreateTableAction(malicious_table_names));
    tDD->setAction(new transitionaction::DropDatabaseAction());
    tDT->setAction(new transitionaction::DropTableAction());
    tMT->setAction(new transitionaction::ModifyTableAction());
    tI->setAction(new transitionaction::InsertAction(ransom_message_values));
    tA->setAction(new transitionaction::AlwaysAction());
    tSV->setAction(new transitionaction::ShowVariableAction());
    tAV->setAction(new transitionaction::SetVariableAction());

    dpn->addTransition(tLD);
    dpn->addTransition(tLT);
    dpn->addTransition(tLC);
    dpn->addTransition(tCT);
    dpn->addTransition(tDD);
    dpn->addTransition(tDT);
    dpn->addTransition(tMT);
    dpn->addTransition(tI);
    dpn->addTransition(tA);
    dpn->addTransition(tAV);
    dpn->addTransition(tSV);

    dpn->addArc(pI1, tLD);
    dpn->addArc(tLD, pI1);
    dpn->addArc(pI2, tLT);
    dpn->addArc(tLT, pI2);
    dpn->addArc(pI3, tLC);
    dpn->addArc(tLC, pI3);

    dpn->addArc(tLD, pOL1);
    dpn->addArc(tLT, pOL1);
    dpn->addArc(tLT, pOL2);
    dpn->addArc(tLC, pOL1);
    dpn->addArc(tLC, pOL2);
    dpn->addArc(tLC, pOL3);

    dpn->addArc(pOL1, tCT);
    dpn->addArc(tCT, pOL1);
    dpn->addArc(pOL1, tDD);
    dpn->addArc(tDD, pOL1);
    dpn->addArc(pOL2, tDT);
    dpn->addArc(tDT, pOL2);
    dpn->addArc(pOL3, tMT);
    dpn->addArc(tMT, pOL3);

    dpn->addArc(tCT, pTC);
    dpn->addArc(tDD, pOD);
    dpn->addArc(tDT, pOD);
    dpn->addArc(tMT, pTC);

    dpn->addArc(pTC, tI);
    dpn->addArc(tI, pTC);

    dpn->addArc(tI, pRI);

    dpn->addArc(pRI, tA);
    dpn->addArc(tA, pRI);
    dpn->addArc(pOD, tA);

    dpn->addArc(tA, pN);

    dpn->addArc(pI4, tAV);
    dpn->addArc(tAV, pI4);
    dpn->addArc(tAV, pVA);

    dpn->addArc(pI5, tSV);
    dpn->addArc(tSV, pI5);
    dpn->addArc(tSV, pVS);

    dpn->setTransitionHelper(new TransitionHelper());
}

void Controller::addClassifier(AbstractClassifier* classifier)
{
    classifier_.push_back(classifier);
}

void Controller::initTriggers(MYSQL_THD thd)
{
    triggerCreator->createTriggers(thd, triggersCreated_);
}

void Controller::handleQuery(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    if( q.getThd() != NULL && q.getActionType() != SQLCOM_SET_OPTION && adminHandler_->getIsAdmin(q.getThd()) )
        return;

    std::list<petrinet::PlaceAction*> executedAction;

    bool executeActionExistsAlready = false;

    for (auto it = std::begin(classifier_); it!=std::end(classifier_); ++it){
        std::list<petrinet::PlaceAction*> created = (*it)->handle(query);

        for (auto iit = std::begin(created); iit!=std::end(created); ++iit){

            if( !*iit)
                continue;

            for (auto iiit = std::begin(executedAction); iiit!=std::end(executedAction); ++iiit){
                if( (*iit)->equals(*(*iiit)) ){
                    executeActionExistsAlready = true;
                    break;
                }
            }

            if( !executeActionExistsAlready ){
                executedAction.push_back(*iit);
            }
        }
    }


    for (auto it = std::begin(executedAction); it!=std::end(executedAction); ++it){
        petrinet::PlaceAction* act = (*it);

        if( act != NULL){
            if( act->getExecuteLater() ){
                const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
                addActionToBeExecutedOnResultSent(q.getThd(), act);
            } else {
                placeaction::BackupAction* ba = dynamic_cast<placeaction::BackupAction*>(act);
                if( ba ){
                    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
                    ba->setTHD(q.getThd());
                }

                placeaction::RewriteQueryAction* rqa = dynamic_cast<placeaction::RewriteQueryAction*>(act);
                if( rqa ){
                    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
                    rqa->setTHD(q.getThd());
                    rqa->setQuery(&q);
                }

                placeaction::AdminModeAction* aaa = dynamic_cast<placeaction::AdminModeAction*>(act);
                if( aaa ){
                    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
                    aaa->setTHD(q.getThd());
                    s_printMessageDirect("AdminModeAction");
                }

                act->execute();
                delete act;
            }
        }
    }
}


/**
  Entry point to the plugin. The server calls this function after each parsed
  query when the plugin is active. The function extracts the digest of the
  query. If the digest matches an existing rewrite rule, it is executed.
*/
int Controller::audit_notify(MYSQL_THD thd, mysql_event_class_t event_class,
                                 const void *event)
{

    if( !triggersCreated_ )
        initTriggers(thd);

  if( event_class == MYSQL_AUDIT_CONNECTION_CLASS ){
      const struct mysql_event_connection *event_connection =
        static_cast<const struct mysql_event_connection *>(event);

      handleConnection(thd, event_connection);

  } else if( event_class == MYSQL_AUDIT_GENERAL_CLASS ){
      const struct mysql_event_general *event_parse=
        static_cast<const struct mysql_event_general *>(event);


        if (event_parse->event_subclass == MYSQL_AUDIT_GENERAL_RESULT ){

            executeActionsOnResultSent(thd);

        }
  } else if( event_class == MYSQL_AUDIT_PARSE_CLASS ){
      const struct mysql_event_parse *event_parse=
        static_cast<const struct mysql_event_parse *>(event);

        if (event_parse->event_subclass == MYSQL_AUDIT_PARSE_POSTPARSE){
            handleEvent(thd, event_class);
        } else if (event_parse->event_subclass == MYSQL_AUDIT_PARSE_PREPARSE){
            actionsExecuteOnResultSentMutex_.lock();
            actionsExecuteOnResultSentMutex_.unlock();
        }
    }

    return 0;
}

void Controller::handleEvent(MYSQL_THD thd, mysql_event_class_t event_class)
{
    DatabasePetriNetQuery query;
    query.setLex(thd->lex);
    std::string q(thd->query().str, thd->query().length);
    query.setQuery(q);
    query.setThd(thd);

    handleQuery(query);

    flushMessageBuffer();
}

void Controller::handleInsertUpdate(UDF_ARGS *args){

    std::string msg;

    if( args->arg_count > 2 ){
        std::string dbname = args->args[0];
        std::string tablename = args->args[1];

        std::vector<std::string> values;

        msg += dbname + ",";
        msg += tablename + ",";

        for( int args_i = 2; args_i < args->arg_count; args_i++){

            std::string value;

            if( args->args[args_i] == NULL)
                continue;

            if( args->arg_type[args_i] == STRING_RESULT) {
                value = args->args[args_i];
                values.push_back(value);
            } else if( args->arg_type[args_i] == INT_RESULT) {
                int val = atoi(args->args[args_i]);
                value = std::to_string(val);
                values.push_back(value);
            } else if( args->arg_type[args_i] == REAL_RESULT || args->arg_type[args_i] == DECIMAL_RESULT) {
                double val = atof(args->args[args_i]);
                value = std::to_string(val);
                values.push_back(value);
            }
        }


        DatabasePetriNetQuery query;
        query.setInsertValues(dbname, tablename, values);
        query.setThd(NULL);

        Controller::handleQuery(query);
    }
}

void Controller::updateExcludedDbsList(const std::string& str)
{
    excluded_dbs_ = split(str, ',');

    if( !vectorContains(excluded_dbs_, storagespace_)){
        excluded_dbs_.push_back(storagespace_);
    }

    std::string msg = "Updated Excluded DB list: " + str;
    s_printMessageDirect(msg);
}

void Controller::updateObjectPrefix(const std::string& str)
{
    object_prefix_ = str;

    std::string msg = "Updated Object Prefix: " + str;
    s_printMessageDirect(msg);
}

void Controller::updateStorageSpace(const std::string& str)
{
    storagespace_ = str;

    if( !vectorContains(excluded_dbs_, storagespace_)){
        excluded_dbs_.push_back(storagespace_);
    }

    std::string msg = "Updated Storage Space: " + str;
    s_printMessageDirect(msg);
}

void Controller::updateSecret(const std::string& str)
{
    secret_ = str;
}

void Controller::flushMessageBuffer()
{
    messageMutex_.lock();
    if( !print_buffer.empty() ){
        print_buffer = "\n" + print_buffer;
        my_plugin_log_message(get_dmaqs_plugin_info(), MY_ERROR_LEVEL, "%s", print_buffer.c_str());
        print_buffer.clear();
    }
    messageMutex_.unlock();
}

void Controller::s_printMessage(const std::string& message)
{
    Controller::s_instance->printMessage(message);
}

void Controller::printMessage(const std::string& message)
{
    messageMutex_.lock();
    if( !print_buffer.empty() ) print_buffer += "\n";
    print_buffer += message;
    messageMutex_.unlock();
}

void Controller::s_printMessageDirect(const std::string& message)
{
    s_printMessage(message);
    Controller::s_instance->flushMessageBuffer();
}

void Controller::addActionToBeExecutedOnResultSent(MYSQL_THD thread, petrinet::PlaceAction* action)
{
    actionsExecuteOnResultSentMutex_.lock();
    actionsExecuteOnResultSent_.emplace(thread, action);
    actionsExecuteOnResultSentMutex_.unlock();
}

void Controller::executeActionsOnResultSent(MYSQL_THD thread)
{
    actionsExecuteOnResultSentMutex_.lock();
    std::multimap<MYSQL_THD,petrinet::PlaceAction*>::iterator it;

    std::list<petrinet::PlaceAction*> executeActions;
    for(it = actionsExecuteOnResultSent_.find(thread); it != actionsExecuteOnResultSent_.end(); it++)
    {
        executeActions.push_back(it->second);
        actionsExecuteOnResultSent_.erase(it);
    }
    for (auto it = std::begin(executeActions); it!=std::end(executeActions); ++it)
    {
        placeaction::CreateTriggerAction* cta = dynamic_cast<placeaction::CreateTriggerAction*>(*it);
        if( cta ){
            cta->setTHD(thread);
        }

        (*it)->execute();
    }

    actionsExecuteOnResultSentMutex_.unlock();
}

void Controller::handleConnection(MYSQL_THD thd, const struct mysql_event_connection *event_connection)
{
    if( event_connection->event_subclass == MYSQL_AUDIT_CONNECTION_CONNECT){
        // on connect
        adminHandler_->addConnection(thd);

    } else if (MYSQL_AUDIT_CONNECTION_DISCONNECT){
        // on disconnect
        adminHandler_->removeConnection(thd);
    }
}

