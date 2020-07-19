
#bootia32.efi

# Stitch byte 0-0xA of boot1-bin into offset 0 of partition
# Stitch byte 0x5A-0x1FD of boot1-bin into offset 0x5A of partition
#  (Avoids overwriting BPB)
# Copy the entire bootfat-bin into sector 2 of the partition
#  (Avoids overwriting FS information sector)

DISK_SIZE_MB=256

$(top_builddir)/fatpart.img: \
		$(top_srcdir)/diskfat.mk \
		\
		dgos-kernel-generic \
		\
		dgos-kernel-tracing \
		\
		dgos-kernel-asan \
		\
		$(top_builddir)/bootx64.efi \
		\
		$(top_builddir)/bootpxe-bios-elf \
		$(top_builddir)/bootpxe-bios-bin \
		$(top_builddir)/bootpxe-bios.map \
		\
		$(top_builddir)/initrd \
		\
		$(top_srcdir)/mkposixdirs.sh \
		\
		$(top_srcdir)/diskfat.mk \
		$(top_srcdir)/populate_fat.sh \
		\
		$(top_builddir)/mbr-bin \
		\
		$(top_builddir)/boot1-bin \
		$(top_builddir)/boot1-elf \
		\
		$(top_builddir)/bootfat-bin \
		\
		$(generate_symbols_outputs) \
		\
		$(MODULE_LIST)
	$(RM) -f $@ \
			$(top_builddir)/mbrdisk.img \
			$(top_builddir)/gptdisk.img && \
		\
		$(TRUNCATE) --size="$(DISK_SIZE_MB)M" \
			"$@" && \
		\
		$(MKFS_VFAT) \
			-F 32 \
			-S $(SECTOR_SZ) \
			-R $$(( 0x10000 / $(SECTOR_SZ) )) \
			-b $$(( 0x10000 / $(SECTOR_SZ) - 1 )) \
			-n DGOS \
			$@ && \
			\
		$(DD) \
			if=$(top_builddir)/boot1-bin \
			of=$@ \
			bs=1 count=$$(( 0xB )) \
			conv=notrunc && \
			\
		$(DD) \
			if=$(top_builddir)/boot1-bin \
			of=$@ \
			bs=1 \
			seek=$$(( 0x5A )) \
			skip=$$(( 0x5A )) \
			count=$$(( 0x1FE - 0x5A )) \
			conv=notrunc && \
			\
		$(DD) \
			if=$(top_builddir)/bootfat-bin \
			of=$@ \
			bs=$(SECTOR_SZ) \
			seek=2 \
			skip=0 \
			conv=notrunc && \
			\
		$(DD) \
			if=$@ \
			of=$@ \
			bs=$(SECTOR_SZ) \
			count=1 \
			skip=0 \
			seek=$$(( 0x10000 / $(SECTOR_SZ) - 1 )) \
			conv=notrunc && \
			\
		SRCDIR="$(top_srcdir)" \
			$(top_srcdir)/populate_fat.sh \
				$(top_builddir)/fatpart.img "$(top_srcdir)"

DGOS_MBRID=0x0615151f
DGOS_UUID=0615151f-d802-4edf-914a-734dc4f03687

$(top_builddir)/mbrdisk.img: fatpart.img
	$(TRUNCATE) --size="$$(( $(DISK_SIZE_MB) + 1 ))M" $@ && \
		\
		printf 'label: dos\nlabel: dos\nlabel-id: %s\n2048,,U,*' \
				"$(DGOS_MBRID)" | \
			$(SFDISK) --no-tell-kernel $@ && \
			\
		$(DD) \
			if=$< \
			of=$@ \
			bs=$(SECTOR_SZ) \
			seek=2048 \
			conv=notrunc && \
			\
		$(DD) \
			if=$(top_builddir)/mbr-bin \
			of=$@ \
			bs=1 \
			count=446 \
			conv=notrunc && \
			\
		$(DD) \
			if=$(top_builddir)/mbr-bin \
			of=$@ \
			bs=1 \
			seek=510 \
			skip=510 \
			count=2 \
			conv=notrunc

$(top_builddir)/hybdisk.img: fatpart.img
	$(TRUNCATE) --size=$$(( $(DISK_SIZE_MB) + 2))M $@ && \
	\
		printf 'label: gpt\n2048,,U,*' | \
			$(SFDISK) $@ && \
		\
		$(DD) \
			if=$(top_builddir)/mbr-bin \
			of=$@ \
			bs=1 \
			count=446 \
			conv=notrunc && \
		\
		$(SGDISK) -A 1:set:2 -h 1 $@ && \
		\
		$(DD) \
			if=$< \
			of=$@ \
			bs=$(SECTOR_SZ) \
			seek=2048 \
			conv=notrunc && \
		\
		$(DD) \
			if=$(top_builddir)/mbr-bin \
			of=$@ \
			bs=1 seek=510 \
			skip=510 \
			count=2 \
			conv=notrunc

$(top_builddir)/gptdisk.img: fatpart.img
	$(TRUNCATE) --size=0 $@ && \
		\
		$(TRUNCATE) --size=$$(( $(DISK_SIZE_MB) + 2))M \
			$@ && \
		\
		$(SGDISK) -Z -n 1:2048:0 -t 1:ef00 -A 1:set:2 \
			$@ && \
		\
		$(DD) \
			if=$< \
			of=$@ \
			bs=$(SECTOR_SZ) \
			seek=2048 \
			conv=notrunc

$(top_builddir)/mbrdisk.qcow: $(top_builddir)/mbrdisk.img
	$(QEMU_IMG) convert -f raw -O qcow -p $< $@

$(top_builddir)/gptdisk.qcow: $(top_builddir)/gptdisk.img
	$(QEMU_IMG) convert -f raw -O qcow -p $< $@

$(top_builddir)/gptdisk.qcow.img: $(top_builddir)/gptdisk.qcow
	$(QEMU_IMG) convert -f qcow -O raw -p $< $@
