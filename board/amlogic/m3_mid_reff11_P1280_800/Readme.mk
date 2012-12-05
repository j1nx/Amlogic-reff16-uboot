m3_mid_reff11_P1280_800_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mid_reff11_P1280_800  amlogic m3
m3_mid_reff11_P1280_800_config_help:
	@echo =======================================================================
	@echo The mark in board is "AML8726-M3_MID_reff11_P1280_800"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mid_reff11_P1280_800_config_help
