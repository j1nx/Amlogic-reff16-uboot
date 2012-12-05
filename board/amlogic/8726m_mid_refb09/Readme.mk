8726m_mid_refb09_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb09 amlogic m1
8726m_mid_refb09_config_help:
	@echo =======================================================================
	@echo The mark in board is "refb09 old MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb09_config_help
