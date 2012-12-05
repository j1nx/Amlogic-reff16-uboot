aml_aio_m2c_haier_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_aio_m2c_haier amlogic m2
	
aml_aio_m2c_haier_config_help:
	@echo =======================================================================
	@echo The mark in board is "HAIER_M2C_V1.0_DDR3_4PC_SI2176 20120112"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_aio_m2c_haier_config_help
