8726m_mid_refb26_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb26 amlogic m1
8726m_mid_refb26_config_help:
	@echo =======================================================================
	@echo The mark in board is "KH MT900 1280*800 MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb26_config_help
