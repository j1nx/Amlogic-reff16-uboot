lenovo_mbox_f16_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson lenovo_mbox_f16  amlogic m3
lenovo_mbox_f16_config_help:
	@echo =======================================================================
	@echo The mark in board is "LENOVO_MBOX_F16 (LENOVO:M3)"
	@echo config command: \"make $(@:%_help=%)\"
help:lenovo_mbox_f16_config_help
