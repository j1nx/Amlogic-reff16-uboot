


a3_dpf_jc_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_dpf_jc amlogic a3

a3_dpf_jc_config_help:
	@echo =======================================================================
	@echo The mark in board is "TOPA3_AV800C_DDR2_V1"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_dpf_jc_config_help
