


a3_dpf_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_socket amlogic a3

a3_dpf_config_help:
	@echo =======================================================================
	@echo The mark in board is "a3_temp"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_dpf_config_help
