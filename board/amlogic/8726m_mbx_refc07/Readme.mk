8726m_mbx_refc07_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mbx_refc07 amlogic m1
8726m_mbx_refc07_config_help:
	@echo =======================================================================
	@echo The mark in board is "Multak 8726M1 MBX"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mbx_refc07_config_help
