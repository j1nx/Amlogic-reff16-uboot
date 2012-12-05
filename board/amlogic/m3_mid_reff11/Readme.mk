m3_mid_reff11_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mid_reff11  amlogic m3
m3_mid_reff11_config_help:
	@echo =======================================================================
	@echo The mark in board is "AML8726-M3_MID_reff11"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mid_reff11_config_help
