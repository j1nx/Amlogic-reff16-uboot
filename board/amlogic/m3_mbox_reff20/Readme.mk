m3_mbox_reff20_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mbox_reff20  amlogic m3
m3_mbox_reff20_config_help:
	@echo =======================================================================
	@echo The mark in board is "MBOX_AML8726_M3"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mbox_reff20_config_help
