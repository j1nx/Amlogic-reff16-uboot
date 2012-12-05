m1_dpf_sz_config: unconfig
	$(MKCONFIG) $(@:_config=) arm aml_meson dpf_aml6236m_sz amlogic m1

m1_dpf_sz_config_help:
	@echo =======================================================================
	@echo The mark in board is "BGA372-M_SKT_V1"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_dpf_sz_config_help
