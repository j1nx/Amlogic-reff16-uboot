


a3_dpf_gadmei_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_dpf_gadmei amlogic a3

a3_dpf_gadmei_config_help:
	@echo =======================================================================
	@echo The mark in board is "P83_A3"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_dpf_gadmei_config_help
