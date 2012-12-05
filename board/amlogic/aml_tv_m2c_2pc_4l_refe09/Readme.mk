aml_tv_m2c_2pc_4l_refe09_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml_tv_m2c_2pc_4l_refe09 amlogic m2
	
aml_tv_m2c_2pc_4l_refe09_config_help:
	@echo =======================================================================
	@echo The mark in board is "BOE_M2C_V1.0_DDR3_2PC_4L_SI2176 20120426"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2c_2pc_4l_refe09_config_help
