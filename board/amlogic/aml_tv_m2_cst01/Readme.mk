aml_tv_m2_cst01_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_tv_m2_cst01 amlogic m2
	
aml_tv_m2_cst01_config_help:
	@echo =======================================================================
	@echo The mark in board is "aml_tv_m2_cst01"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2_cst01_config_help
