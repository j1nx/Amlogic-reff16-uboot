m3_oplay_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_oplay  amlogic m3
m3_oplay_config_help:
	@echo =======================================================================
	@echo The mark in board is "OPLAY_AML8726_M_R100 (ASUS:M3)"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_oplay_config_help
