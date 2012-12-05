aml_tv_m2c_refe03_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_tv_m2c_refe03 amlogic m2
	
aml_tv_m2c_refe03_config_help:
	@echo =======================================================================
	@echo The mark in board is "TFZ7366M_HC"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2c_refe03_config_help
