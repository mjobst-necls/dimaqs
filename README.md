# Dimaqs

## How to build the project
    
    # Clone the repository
    git clone --recurse-submodules git@github.com:mjobst-necls/dimaqs.git
    
    # Build the docker container from within the root directory
    docker build -t dimaqs .
    
    # Run the docker container
    docker run -it dimaqs /bin/sh
