8726m_mid_refb03GPS_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb03GPS amlogic m1
8726m_mid_refb03GPS_config_help:
	@echo =======================================================================
	@echo The mark in board is "KH 7inch MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb03GPS_config_help
