m3_mbox_reff34_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mbox_reff34  amlogic m3
m3_mbox_reff34_config_help:
	@echo =======================================================================
	@echo The mark in board is "GNTV MBOX_AML8726_M3"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mbox_reff34_config_help
