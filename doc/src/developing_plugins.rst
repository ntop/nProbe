Developing nProbe Plugins
#########################

Each nProbe plugin is implemented as shared library to be loaded at runtime by nProbe. The probe comes with several plugins that can be used as example for this activity. Below we list the main concepts you need to know if you plan to develop nProbe plugins.

Each plugin has to defined a plugin entry point as follows

.. code:: c

     static PluginEntryPoint dbPlugin = {
       NPROBE_REVISION,
       "My Plugin Name",
       "shortName", NULL,
       "version",
       "Plugin string description",
       "author email",
       1 /* do NOT skip in collector mode */,
       0 /* always enabled */, 1, /* enabled */
       PLUGIN_DONT_NEED_LICENSE,
       myPlugin_init,
       NULL, /* Term */
       myPlugin_conf,
       myPlugin_delete,
       1, /* call packetFlowFctn for each packet */
       NULL /* myPlugin_packet */,
       myPlugin_get_template,
       myPlugin_export,
       myPlugin_print,
       NULL,
       NULL,
       myPlugin_help,
       NULL, 0, 0
     };
     
and a function with the following format

.. code:: c

     #ifdef MAKE_STATIC_PLUGINS
     PluginEntryPoint* myPluginEntryFctn(void)
     #else
     PluginEntryPoint* PluginEntryFctn(void)
     #endif
     {
       return(&myPlugin);
     }
     
The fields of the PluginEntryPoint function have the following meaning:

  - :code:`char *nprobe_revision`; string to be defined as NPROBE_REVISION.
  - :code:`char *name`; Extended plugin name.
  - :code:`char *short_name`; Short plugin name.
  - :code:`char *family`; Plugin family name (if any) or NULL to use the short plugin name.
  - :code:`char *version`; Plugin version (e.g. 1.0).
  - :code:`char *descr`; Plugin description in plain English.
  - :code:`char *author`; Plugin author name and email.
  - :code:`u_int8_t skip_in_collector_mode`; Set it to 0 to skip the plugin when nprobe is in collector mode, set it to 1 otherwise.
  - :code:`u_int8_t always_enabled`; Set it to 1 to enable the plugin permanently regardless of its use in the template.
  - :code:`u_int8_t enabled`; Do not touch it and set it to 0 (used by nProbe).
  - :code:`u_int8_t need_license`; Set it to 1 if a license for this plugin is needed, or 0 if is not needed.
  - :code:`PluginInitFctn initFctn`; Plugin initialization function called when the plugin is loaded in memory. This function is called regardless of the fact that the plugin will later be used or not.
  - :code:`PluginTermFctn termFctn`; Plugin termination function called when the plugin is terminated during nProbe shutdown.
  - :code:`PluginConf pluginFlowConf`; Function that returns the flow configuration (see below).
  - :code:`PluginFctn deleteFlowFctn`; Flow callback that is called for flows handled by this plugin whenever a flow has been exported. This function is used to free memory of resources associated to the flow. Set it to NULL if no function will be defined, 
  - :code:`u_int8_t call_packetFlowFctn_for_each_packet`; Set it to 1 to ask nProbe to call the packetFlowFctn callback for every packet belonging to this flow, or 0 for calling it only for the first flow packet.
  - :code:`PluginPacketFctn packetFlowFctn`; Callback called whenever nProbe has a packet belonging to the flow to be processed by the plugin.
  - :code:`PluginGetTemplateFctn getTemplateFctn`; Function used to return the template Element for the specified information element passed as parameter.
  - :code:`PluginExportFctn pluginExportFctn`; Callback called whenever the flow handled by this plugin is going to be exported.
  - :code:`PluginPrintFctn pluginPrintFctn`; Function that is called when nprobe -P is used, and that is supposed to print flow information into text files.
  - :code:`PluginStatsFctn pluginStatsFctn`; Function that is called (when not set to NULL) whenever nProbe prints periodic information (-b 1 or -b 2).
  - :code:`PluginSetupFctn setupFctn`; Function called after plugin initialization (when not set to NULL), if according to the specified template, this plugin will be used.
  - :code:`PluginHelpFctn helpFctn`; Function that is called when nprobe -h is executed, and that is supposed to print plugin information.
  - :code:`PluginIdleTaskFctn idleFctn`; If not set to NULL, this function will be periodically called by the nProbe core to execute (if any) housekeeping activities.
  - :code:`u_int8_t v4TemplateIdx, v6TemplateIdx`; Used by nProbe. Set them to 0.


Each plugin must define a template with the following format

.. code:: c

     static V9V10TemplateElementId myPlugin_template[] = {
       { 0, BOTH_IPV4_IPV6, FLOW_TEMPLATE, LONG_SNAPLEN, NTOP_ENTERPRISE_ID, 0, STATIC_FIELD_LEN, 0, 0, 0, NULL, NULL, NULL }
     };

what will be then used by the following functions

.. code:: c

     static V9V10TemplateElementId* myPlugin_get_template(char* template_name) {
       int i;
     
       for(i=0; myPlugin_template[i].templateElementId != 0; i++) {
         if(!strcmp(template_name, myPlugin_template[i].netflowElementName)) {
           return(&myPlugin_template[i]);
         }
       }
     
       return(NULL); /* Unknown */
     }
     
     static V9V10TemplateElementId* myPlugin_conf(void) {
       return(myPlugin_template);
     }
     

In the file template.h are specified the flow identifiers to be used in V9V10TemplateElementId that is defined as follows:

  - :code:`u_int8_t isInUse`; Always set it to 1, or 0 if it is the last template element to indicate that no further element will be defined.
  - :code:`u_int8_t protoMode`; Set it to BOTH_IPV4_IPV6 or ONLY_IPV4, ONLY_IPV6 if this element is for both IPv4 and IPv46 flows, just for IPv4 flows, or just for IPv6 flows.
  - :code:`const u_int8_t isOptionTemplate`; Set it to 0 if this is a flow template (default), or 1 if it used as option template.
  - :code:`const u_int8_t useLongSnaplen`; Set it to 1 if this plugin requires nProbe to capture packets with long snaplen that are needed when the plugin has to perform payload analysis.
  - :code:`const u_int32_t templateElementEnterpriseId`; Specify the IANA defined enterprise Id for this custom field. ntop uses NTOP_ENTERPRISE_ID for the proprietary ones.
  - :code:`const u_int16_t templateElementId`; Used by nProbe, leave it to 0.
  - :code:`u_int8_t variableFieldLength`; Set it to 1 to indicate that if nProbe exports flows in IPFIX format (-V 10) this field will have a variable field size.
  - :code:`u_int16_t templateElementLen`; Specify the static field size (-V 9) or max field size (-V 10)
  - :code:`const ElementFormat elementFormat`; Specify the format of the element. This information will be used when this data is printed into MySQL. The supported format types are: ascii_format, hex_format, numeric_format, ipv6_address_format.
  - :code:`const ElementDumpFormat fileDumpFormat`; Specify the field format when the nProbe metadata information is printed (--metadata). The supported format types are: dump_as_uint , dump_as_formatted_uint, dump_as_ip_port, dump_as_ip_proto, dump_as_ipv4_address, dump_as_ipv6_address, dump_as_mac_address, dump_as_epoch, dump_as_bool, dump_as_tcp_flags, dump_as_hex, dump_as_ascii
  - :code:`const char *netflowElementName`; String with the symbolic network element name used in NetFlow (-V 9).
  - :code:`const char *ipfixElementName`; String with the symbolic network element name used in IPFIX (-V 10).
  - :code:`const char *templateElementDescr`; String that describes the element information type used by nProbe when the help (-h) is printed.

  
Most plugin callbacks are straightforward and its logic can be understood simply having a look at examples of existing plugins. The only function worth to describe is the one that processes packets as it is the most complex one.

.. code:: c

     static void myPlugin_packet(u_char new_bucket,
                         int packet_if_idx /* -1 = unknown */,
                         void *pluginData,
                         FlowHashBucket* bkt,
                         FlowDirection flow_direction,
                         u_int16_t ip_offset, u_short proto,
                         u_char isFragment,
                         u_short numPkts, u_char tos,
                         u_short vlanId, struct eth_header *ehdr,
                         IpAddress *src, u_short sport,
                         IpAddress *dst, u_short dport,
                         u_int len, u_int8_t flags,
                         u_int32_t tcpSeqNum,  u_int8_t icmpType,
                         u_short numMplsLabels,
                         u_char mplsLabels[MAX_NUM_MPLS_LABELS [MPLS_LABEL_LEN],
                         const struct pcap_pkthdr *h, const u_char *p,
                         u_char *payload, int payloadLen) {
        ...
     }
     

This function processes a packet belonging to a flow handled by this plugin. nProbe has no clue what plugins are doing, this whenever a new flow is created (new_bucket is set to 1 for the first packet of the flow, or 0 for the following packets), it calls all active plugins to tell that a new flow is active in cache. The plugin will then decide if the packet can be handled by the plugin or not. This is done by looking at the packet header fields passed to the function, or inspecting the packet payload (payload point whose length is specified by payloadLen). If a plugin decides that the packet cannot be handled by the plugin (for instance because the packet protocol is not managed by the plugin) no action is needed and the function must simply return. Instead if the plugin can handle the packet, at the beginning of the function the following code-like must be specified in order to add the plugin to the list of plugins (it should usually be 1 or 0 element long) handling this flow.

.. code:: c

     if(new_bucket /* This bucket has been created recently */) {
     
           info->pluginPtr  = (void*)&myPlugin;
     pluginData = info->pluginData = (struct my_plugin_info*)malloc(sizeof(struct my_plugin_info));
     
           if(info->pluginData == NULL) {
             traceEvent(TRACE_ERROR, "Not enough memory?");
             free(info);
             return; /* Not enough memory */
           } else {
             struct my_plugin_info *myinfo = (struct my_plugin_info*)pluginData;
     
             /* Reset fields */
             memset(myinfo, 0, sizeof(struct my_plugin_info));
     
             info->next = bkt->ext->plugin;
             info->plugin_used = 0;
             bkt->ext->plugin = info;
           }
         }
       }
     
Once a plugin is defined, it must be placed into the nProbe/plugins directory so that the nProbe build process will detect and compile it.

Following is a complete example of a plugin with its callbacks.

.. code:: c

          /*
           *
           *  This is a dummy plugin example
           *
           */
          
          #include "nprobe.h"
          
          static V9V10TemplateElementId dummyPlugin_template[] = {
          							{ 0, BOTH_IPV4_IPV6, FLOW_TEMPLATE, LONG_SNAPLEN, NTOP_ENTERPRISE_ID, 0, STATIC_FIELD_LEN, 0, 0, 0, NULL, NULL, NULL }
          };
          
          struct dummy_plugin_info {
            u_int8_t dummy_value;
          };
          
          /* *********************************************** */
          
          static PluginEntryPoint dummyPlugin; /* Forward */
          
          
          /* ******************************************* */
          
          static void dummyPlugin_init() {
            traceEvent(TRACE_NORMAL, "Initialized dummy plugin");
          }
          
          /* *********************************************** */
          
          /* Handler called whenever an incoming packet is received */
          
          static void dummyPlugin_packet(FlowCallback callback,
          			       int packet_if_idx /* -1 = unknown */,
          			       void *pluginData,
          			       FlowHashBucket* bkt,
          			       FlowDirection flow_direction,
          			       u_int16_t ip_offset, u_int16_t proto, u_char isFragment,
          			       u_int16_t numPkts, u_char tos, u_int8_t retransmitted_pkt,
          			       u_int16_t vlanId, struct eth_header *ehdr,
          			       IpAddress *src, u_int16_t sport,
          			       IpAddress *dst, u_int16_t dport,
          			       u_int plen, u_int8_t flags,
          			       u_int32_t tcpSeqNum, u_int8_t icmpType,
          			       u_int16_t numMplsLabels,
          			       u_char mplsLabels[MAX_NUM_MPLS_LABELS][MPLS_LABEL_LEN],
          			       const struct pcap_pkthdr *h, const u_char *p,
          			       u_char *_priv_payload, int payloadLen) {
            traceEvent(TRACE_NORMAL, "%s()", __FUNCTION__);
          }
          
          /* *********************************************** */
          
          /* Handler called when the flow is deleted (after export) */
          
          static void dummyPlugin_delete(FlowHashBucket* bkt, void *pluginData) {
            if(pluginData)
              free(pluginData);
          }
          
          /* *********************************************** */
          
          /* Handler called at startup when the template is read */
          
          static V9V10TemplateElementId* dummyPlugin_get_template(char* template_name) {
            int i;
          
            for(i=0; dummyPlugin_template[i].templateElementId != 0; i++) {
              if(!strcmp(template_name, dummyPlugin_template[i].netflowElementName)) {
                return(&dummyPlugin_template[i]);
              }
            }
          
            return(NULL); /* Unknown */
          }
          
          /* *********************************************** */
          
          /* Handler called whenever a flow attribute needs to be exported */
          
          static int dummyPlugin_export(void *pluginData, V9V10TemplateElementId *theTemplate,
          			      FlowDirection direction /* 0 = src->dst, 1 = dst->src */,
          			      FlowHashBucket *bkt, char *outBuffer,
          			      uint* outBufferBegin, uint* outBufferMax) {
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
            return(-1); /* Not handled */
          }
          
          /* *********************************************** */
          
          /* Handler called whenever a flow attribute needs to be printed on file */
          
          static int dummyPlugin_print(void *pluginData, V9V10TemplateElementId *theTemplate,
          			     FlowDirection direction /* 0 = src->dst, 1 = dst->src */,
          			     FlowHashBucket *bkt, char *line_buffer, uint line_buffer_len,
          			     u_int8_t json_mode, u_int8_t *escape_string) {
            int len;
          
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
          
            if(!pluginData) return(-1);
          
            switch(theTemplate->templateElementId) {
            default:
              return(-1); /* Not handled */
            }
          
            return(len);
          }
          
          
          /* *********************************************** */
          
          static V9V10TemplateElementId* dummyPlugin_conf(void) {
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
            return(dummyPlugin_template);
          }
          
          /* *********************************************** */
          
          static void dummyPlugin_term(void) {
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
          }
          
          /* *********************************************** */
          
          static void dummyPlugin_help(void) {
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
          }
          
          /* *********************************************** */
          
          static void dummyPlugin_idle(void) {
            traceEvent(TRACE_INFO, "%s()", __FUNCTION__);
          }
          
          /* *********************************************** */
          
          static const struct option plugin_options[] = {
          					       { "dummy-option",           required_argument, NULL, 252 /* dummy */ },
          					       { NULL,                     no_argument,       NULL,   0 }
          };
          
          static const struct option* dummyPlugin_opts() {
            return(plugin_options);
          }
          
          /* *********************************************** */
          
          /* Plugin entrypoint */
          static PluginEntryPoint dummyPlugin = {
          				       NPROBE_REVISION,
          				       "Dummy Protocol",
          				       "dummy", NULL, NULL,
          				       "0.1",
          				       "Handle Dummy protocol",
          				       "developer@company.com",
          				       0 /* not always enabled */, 1, /* enabled */
          				       PLUGIN_NEED_LICENSE,
          				       PLUGIN_STANDARD_VERSION /* pro/standard plugin version */,
          				       dummyPlugin_init,
          				       dummyPlugin_opts,
          				       dummyPlugin_term,
          				       dummyPlugin_conf,
          				       dummyPlugin_delete,
          				       1, /* call packetFlowFctn for each packet */
          				       dummyPlugin_packet,
          				       dummyPlugin_get_template,
          				       dummyPlugin_export,
          				       dummyPlugin_print,
          				       NULL,
          				       NULL,
          				       dummyPlugin_help,
          				       dummyPlugin_idle,
          				       0, 0
          };
          
          /* *********************************************** */
          
          /* Plugin entry fctn */
          #ifdef MAKE_STATIC_PLUGINS
          PluginEntryPoint* dummyPluginEntryFctn(void)
          #else
            PluginEntryPoint* PluginEntryFctn(void)
          #endif
          {
            return(&dummyPlugin);
          }
          
