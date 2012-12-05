8726m_mid_refb09new_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb09new amlogic m1
8726m_mid_refb09new_config_help:
	@echo =======================================================================
	@echo The mark in board is "refb09 new MID"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb09new_config_help
