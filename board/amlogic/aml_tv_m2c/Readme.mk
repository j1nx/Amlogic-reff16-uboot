aml_tv_m2c_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_tv_m2c amlogic m2
	
aml_tv_m2c_config_help:
	@echo =======================================================================
	@echo The mark in board is "M2_SOCKET_VER_C_V1 201108019"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2c_config_help
