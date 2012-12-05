m1_mbx_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson mbx_aml8626m_skt_v1 amlogic m1




m1_mbx_config_help:
	@echo =======================================================================
	@echo The mark in board is "m2_socket_v1"
	@echo config command: \"make $(@:%_help=%)\"
	
help:m1_mbx_config_help
