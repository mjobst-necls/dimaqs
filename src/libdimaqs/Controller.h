/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ANUBIS_H
#define ANUBIS_H

#include <my_global.h>
#include <mysql/plugin_audit.h>

#include "string"
#include "list"
#include "vector"
#include "map"
#include "mutex"
#include "AdminHandler.h"

class AbstractClassifier;
class TriggerCreator;
class QueryRewriter;

namespace petrinet{
class AbstractPetriNetQuery;
class PlaceAction;
}

class Controller
{
public:
    Controller();
    virtual ~Controller();

    void addClassifier(AbstractClassifier* classifier);

    int audit_notify(MYSQL_THD thd, mysql_event_class_t event_class, const void *event);
    void handleEvent(MYSQL_THD thd, mysql_event_class_t event_class);
    void handleInsertUpdate(UDF_ARGS *args);

    void handleQuery(const petrinet::AbstractPetriNetQuery& query);

    virtual void printMessage(const std::string& message);
    void flushMessageBuffer();

    void initTriggers(MYSQL_THD thd);
    void updateExcludedDbsList(const std::string& str);
    void updateObjectPrefix(const std::string& str);
    void updateStorageSpace(const std::string& str);
    void updateSecret(const std::string& str);
    void updateVerbosity(uint verbosity) { verbosity_ = verbosity; }
    void updateRecipient(const std::string& str) { recipient_ = str; }


    inline static uint getVerbosity() { return s_instance->verbosity_; }

    virtual void init();

    static Controller* s_instance;

    static void s_printMessage(const std::string& message);
    static void s_printMessageDirect(const std::string& message);

    void addActionToBeExecutedOnResultSent(MYSQL_THD thread, petrinet::PlaceAction* action);
    void executeActionsOnResultSent(MYSQL_THD thread);

protected:
    void handleConnection(MYSQL_THD thd, const struct mysql_event_connection *event_connection);



private:
    std::list<AbstractClassifier*>  classifier_;
    std::string                     print_buffer;

    TriggerCreator*                 triggerCreator;

    bool                            triggersCreated_;
    QueryRewriter*                  queryRewriter_;

// variables
    std::vector<std::string>        excluded_dbs_;
    std::string                     object_prefix_;
    std::string                     storagespace_;
    std::string                     secret_;
    std::string                     recipient_;
    uint                            verbosity_;
    std::mutex                      actionsExecuteOnResultSentMutex_;
    std::multimap<MYSQL_THD, petrinet::PlaceAction*> actionsExecuteOnResultSent_;
    AdminHandler*                   adminHandler_;

    std::mutex                      messageMutex_;

};

#endif // ANUBIS_H
