Custom Plugin
#############

nProbe exports flows according to the fields specified in the template with option -T (or the default template).
The full list of information elements notively supported by nProbe can be generated with :code:`nprobe -H`.
This plugin adds the ability to define custom information elements and assign a value to them, that can be:

- a literal string
- a literal number
- the result of a function applied to the actual flow

Custom fields can be specified using :code:`--custom-fields <fields>`, where <fields> is a comma-separated list
of <key>=<value> pairs. Field keys should be added to the template as usual using the :code:`-T <elements>` option.

For instance, in the example below, we extend the @NTOPNG@ template with a few custom fields and export those 
custom flows to ntopng:

.. code:: bash

   nprobe -i eth0 --custom-fields "FIELD1=field1,FIELD2=22,FIELD3=dummyFunction()" -T "@NTOPNG@ %FIELD1 %FIELD2 %FIELD3" --zmq tcp://*:5556

Where:

- FIELD1 is defined as the literal string 'field1'
- FIELD2 is defined as the literal number '22'
- FIELD3 is defined as the function dummyFunction()

