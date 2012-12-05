8006m_mid_refb17_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8006m_mid_refb17 amlogic m1
8006m_mid_refb17_config_help:
	@echo =======================================================================
	@echo The mark in board is "PAN8006_MOON"
	@echo config command: \"make $(@:%_help=%)\"
help:8006m_mid_refb17_config_help
