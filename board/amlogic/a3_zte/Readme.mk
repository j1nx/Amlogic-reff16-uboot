


a3_zte_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_zte amlogic a3

a3_zte_config_help:
	@echo =======================================================================
	@echo The mark in board is "a3_zte"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_zte_config_help
