package=libsodium
$(package)_version=1.0.18
$(package)_download_path=https://download.libsodium.org/libsodium/releases/
$(package)_file_name=$(package)-$($(package)_version)-stable.tar.gz
$(package)_sha256_hash=2ef4304dd844e274d78dfcf0201e1ba1a312eaba135bdc8affc56a3d80b58593

define $(package)_set_vars
$(package)_config_opts_mingw32+=CFLAGS="-Ofast -fomit-frame-pointer -march=pentium3 -mtune=westmere"
endef

define $(package)_config_cmds
  ./configure --host=$(HOST) --prefix=$(host_prefix) --exec-prefix=$(host_prefix)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
