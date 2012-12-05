8726m_mid_refb02_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb02 amlogic m1
8726m_mid_refb02_config_help:
	@echo =======================================================================
	@echo The mark in board is "WA 10inch MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb02_config_help
