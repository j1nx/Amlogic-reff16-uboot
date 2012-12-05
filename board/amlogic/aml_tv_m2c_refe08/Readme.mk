aml_tv_m2c_refe08_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_tv_m2c_refe08 amlogic m2
	
aml_tv_m2c_refe08_config_help:
	@echo =======================================================================
	@echo The mark in board is "HAIER_M2C_V1.0_DDR3_4PC_SI2176 20120112"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2c_refe08_config_help
