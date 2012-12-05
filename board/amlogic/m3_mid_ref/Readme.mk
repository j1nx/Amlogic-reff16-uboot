m3_mid_ref_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mid_ref  amlogic m3
m3_mid_ref_config_help:
	@echo =======================================================================
	@echo The mark in board is "AML8726-M3_MID_Ref"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mid_ref_config_help
