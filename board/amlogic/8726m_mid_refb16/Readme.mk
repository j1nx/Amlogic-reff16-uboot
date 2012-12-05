8726m_mid_refb16_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb16 amlogic m1
8726m_mid_refb16_config_help:
	@echo =======================================================================
	@echo The mark in board is "tuxing 9.7inch MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb16_config_help
