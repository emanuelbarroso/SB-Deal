/**
 * @author Emanuel B.
 * Elf Creator class source code
 *
**/

#include "ElfCreator.h"

ElfCreator::ElfCreator(string file,vector<unsigned char> t,vector<unsigned char> d,int ts,int ds,SymbolTable sym) {
	textStart = ts;
	dataStart = ds;
	symTable = sym;
	fileDsc = file;
	textSize = t.size();
	dataSize = d.size();
	textSeg = new char[t.size()];
	dataSeg = new char[d.size()];

	//cout << "Elf called for:" << endl;
	for (unsigned i = 0;i < t.size();i++) {
		textSeg[i] = t.at(i);
		cout << setfill('0');
		//cout << "Tx" << hex << setw(2) << uppercase << static_cast<int>(textSeg[i]) << " ";
	}
	//cout << endl;
	for (unsigned i = 0;i < d.size();i++) {
		dataSeg[i] = d.at(i);
		cout << setfill('0');
		//cout << "Dx" << hex << setw(2) << uppercase << static_cast<int>(dataSeg[i]) << " ";
	}
	//cout << endl;
}

ElfCreator::~ElfCreator() {
	delete [] textSeg;
	delete [] dataSeg;
}

void ElfCreator::elfCreate() {
	elfio writer;

	writer.create( ELFCLASS32, ELFDATA2LSB );
	writer.set_os_abi( ELFOSABI_LINUX );
	writer.set_type( ET_EXEC );
	writer.set_machine( EM_386 );

	section *text_sec = writer.sections.add(".text");
	text_sec->set_type(SHT_PROGBITS);
	text_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
	text_sec->set_addr_align(0x10);
	text_sec->set_data(textSeg,textSize);

	segment *text_seg = writer.segments.add();
	text_seg->set_type(PT_LOAD);
	text_seg->set_virtual_address(textStart);
	text_seg->set_physical_address(textStart);
	text_seg->set_flags(PF_X | PF_R);
	text_seg->set_align(0x1000);
	text_seg->add_section_index(text_sec->get_index(),text_sec->get_addr_align());
	
	section *data_sec = writer.sections.add(".data");
	data_sec->set_type(SHT_PROGBITS);
	data_sec->set_flags(SHF_ALLOC | SHF_WRITE);
	data_sec->set_addr_align(0x4);
	data_sec->set_data(dataSeg,dataSize);

    segment *data_seg = writer.segments.add();
	data_seg->set_type(PT_LOAD);
	data_seg->set_virtual_address(dataStart);
	data_seg->set_physical_address(dataStart);
	data_seg->set_flags(PF_W | PF_R);
	data_seg->set_align(0x10);
	data_seg->add_section_index(data_sec->get_index(), data_sec->get_addr_align());

	section *note_sec = writer.sections.add(".note");
	note_sec->set_type(SHT_NOTE);
	note_sec->set_addr_align(1);
	note_section_accessor note_writer(writer,note_sec);
	note_writer.add_note(0x01,"Author: Emanuel B.",0,0);

	section* str_sec = writer.sections.add(".strtab");
    str_sec->set_type(SHT_STRTAB);
    string_section_accessor stra(str_sec);

	section *sym_sec = writer.sections.add(".symtab");
	sym_sec->set_type(SHT_SYMTAB);
	sym_sec->set_info(2);
	sym_sec->set_addr_align(0x4);
	sym_sec->set_entry_size(writer.get_default_entry_size(SHT_SYMTAB));
	sym_sec->set_link(str_sec->get_index());
	symbol_section_accessor syma(writer,sym_sec);

	syma.add_symbol(stra,"_start",textStart,0,STB_WEAK,STT_FUNC,0,text_sec->get_index());

	vector<string> symEntries = symTable.getEntries();
	for (string &k : symEntries) {
		Elf32_Word str_index = stra.add_string(k.c_str());
		if (!symTable.isDataSymbol(k)) {
			syma.add_symbol(str_index,symTable.getAddress(k),0,STB_WEAK,STT_FUNC,0,text_sec->get_index());
		}
		else {
			syma.add_symbol(str_index,symTable.getAddress(k),0,STB_GLOBAL,STT_OBJECT,0,data_sec->get_index());
		}
	}

	writer.set_entry(textStart);
	writer.save(fileDsc);

	string command = "chmod +x " + fileDsc;
	system(command.c_str());
}
