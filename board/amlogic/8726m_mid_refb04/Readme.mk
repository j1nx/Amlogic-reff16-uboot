8726m_mid_refb04_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb04 amlogic m1
8726m_mid_refb04_config_help:
	@echo =======================================================================
	@echo The mark in board is "KH 8inch MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb04_config_help
