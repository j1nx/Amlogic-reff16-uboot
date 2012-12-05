8726m_mid_refb01_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb01 amlogic m1
8726m_mid_refb01_config_help:
	@echo =======================================================================
	@echo The mark in board is "WA 7inch MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb01_config_help
