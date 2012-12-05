m1_mbox_m1_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson mbox_8626m1 amlogic m1
m1_mbox_m1_config_help:
	@echo =======================================================================
	@echo The mark in board is "MBOX_8626M1_V1.0"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_mbox_m1_config_help
