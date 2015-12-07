Kibana4 dashboard dump tool
===========================

Simple tool to dump / save / backup your Kibana4 dashboards.

Install
-------

     virtualenv env
     . env/bin/activate
     pip install -r requirements.txt

Usage example
-------------

      python kibana_dump.py --url 'http://localhost:9200' --dir output
