


a3_ref_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_ref amlogic a3

a3_ref_config_help:
	@echo =======================================================================
	@echo The mark in board is "a3_ref"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_ref_config_help
