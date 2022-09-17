FROM ubuntu:22.04

WORKDIR /app

RUN apt update
RUN apt install curl wget python3 python3-pip -y

RUN wget https://downloads.mariadb.com/MariaDB/mariadb_repo_setup
RUN chmod +x mariadb_repo_setup
RUN ./mariadb_repo_setup --mariadb-server-version="mariadb-10.6"
RUN apt install libmariadb3 libmariadb-dev -y

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY . .

CMD [ "python3", "fetcher.py" ]
