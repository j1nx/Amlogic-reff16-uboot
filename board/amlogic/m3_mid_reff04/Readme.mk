m3_mid_reff04_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mid_reff04  amlogic m3
m3_mid_reff04_config_help:
	@echo =======================================================================
	@echo The mark in board is "AML8726-M3_MID_REFF04"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mid_reff04_config_help
