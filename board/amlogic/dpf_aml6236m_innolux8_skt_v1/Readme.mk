


m1_dpf_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson dpf_aml6236m_innolux8_skt_v1 amlogic m1

m1_dpf_config_help:
	@echo =======================================================================
	@echo The mark in board is "m2_socket_v1"
	@echo config command: \"make $(@:%_help=%)\"
	
help:m1_dpf_config_help
