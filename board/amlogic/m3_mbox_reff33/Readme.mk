m3_mbox_reff33_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_mbox_reff33  amlogic m3
m3_mbox_reff33_config_help:
	@echo =======================================================================
	@echo The mark in board is "PBI MBOX_AML8726_M3"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_mbox_reff33_config_help
