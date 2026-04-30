#ifndef FILE_HEADER
#define FILE_HEADER

// TODO:
// check file permissions before opening
// add permissions to file on open

#include "define.h"

#if defined(CPP_17_SUPPORTED)
#include <filesystem>
#endif

#include <memory>

#include <Windows.h>

#include "Exceptions.hpp"

namespace {

	class BasicFile {
	protected:

		FILE* Stream;
		mutable long FileSize = -1;

		void Seek(long Offset, int Mode = SEEK_SET) const;

	public:
		inline BasicFile();
		inline ~BasicFile();

		void CloseFile();

		long GetSize() const;
	};

	BasicFile::BasicFile() {
		this->Stream = nullptr;
	}

	BasicFile::~BasicFile() {
		this->CloseFile();
	}


	void BasicFile::Seek(long Offset, int Mode) const {

		unlikely_branch
		if (fseek(this->Stream, Offset, Mode)) {
			Exceptions::ThrowException<Exceptions::IOError>("Failed to seek to the end of a FileStream.");
		}
	}


	void BasicFile::CloseFile() {

		if (this->Stream == nullptr)
			return;

		fclose(this->Stream);
		this->Stream = nullptr;
	}

	long BasicFile::GetSize() const {

		if (this->FileSize == -1) {
			this->Seek(0, SEEK_END);
			this->FileSize = ftell(this->Stream);
			this->Seek(0);
		}
		return this->FileSize;
	}
}

namespace File {

	inline void Init();

	std::unique_ptr<char[]> ReadTextFile(const char* Filename, size_t* FileSize = nullptr);



	class FileReader : public BasicFile {
	public:
		inline FileReader(const char* Filename);

		void OpenFile(const char* Filename);

		void ReadIntoBuffer(void* Buffer, size_t ReadBytes, long Offset);
		void ReadIntoBuffer_s(void* Buffer, size_t ReadBytes, long Offset, size_t BufferSize);
		std::unique_ptr<char[]> Read(size_t ReadBytes, long Offset);

		inline std::unique_ptr<char[]> ReadAll();
	};


	class FileWriter : public BasicFile {
	public:
		inline FileWriter(const char* Filename);

		void OpenFile(const char* Filename);

		void Write(const void* Buffer, size_t WriteBytes, long Offset);
	};

	
}


void File::Init() {

	char Buffer[512];

	// GetModuleFileNameA returns length of string
	size_t i = GetModuleFileNameA(NULL, Buffer, sizeof(Buffer));

	for (; Buffer[i] != '\\'; --i);

	Buffer[i] = '\0';

#if defined(CPP_17_SUPPORTED)
	std::filesystem::current_path(Buffer);
#else
	SetCurrentDirectoryA(Buffer);
#endif
}

	// Read a text file into a buffer, null-terminating it at the end.
std::unique_ptr<char[]> File::ReadTextFile(const char* Filename, size_t* FileSize) {

	File::FileReader Reader(Filename);

	const size_t Size = Reader.GetSize();
	if (FileSize != nullptr) {
		*FileSize = Size;
	}

	std::unique_ptr<char[]> Buffer = std::make_unique<char[]>(Size + 1);
	Reader.ReadIntoBuffer(Buffer.get(), Size, 0);

	Buffer.get()[Size] = '\0';

	std::cout << Buffer.get() << '\n';

	return std::move(Buffer);
}


File::FileReader::FileReader(const char* Filename) {
	this->OpenFile(Filename);
}

void File::FileReader::OpenFile(const char* Filename) {

	//std::filesystem::perms FilePermissions = std::filesystem::directory_entry(Filename).status().permissions();
	//if (FilePermissions & std::filesystem::perms::others_read)
	//std::filesystem::perms::read

	fopen_s(&this->Stream, Filename, "rb");
	if (this->Stream == nullptr) {
		Exceptions::ThrowException<Exceptions::IOError>("Failed to open a file for reading.");
	}
	/*
	std::filesystem::permissions(
		Filename,
		std::filesystem::perms::group_read
	);*/
}

void File::FileReader::ReadIntoBuffer(void* Buffer, size_t ReadBytes, long Offset) {

	this->Seek(Offset);
	fread(Buffer, 1, ReadBytes, this->Stream);
	this->Seek(0);
}

void File::FileReader::ReadIntoBuffer_s(void* Buffer, size_t ReadBytes, long Offset, size_t BufferSize) {

	if (ReadBytes > BufferSize) {
		Exceptions::ThrowException<Exceptions::OverflowError>("Attempted to read more bytes than that could fit inside the buffer.");
	}

	this->ReadIntoBuffer(Buffer, ReadBytes, Offset);
}

std::unique_ptr<char[]> File::FileReader::Read(size_t ReadBytes, long Offset) {

	std::unique_ptr<char[]> Buffer = std::make_unique<char[]>(ReadBytes);
	this->ReadIntoBuffer(Buffer.get(), ReadBytes, Offset);
	return std::move(Buffer);
}

std::unique_ptr<char[]> File::FileReader::ReadAll() {
	return this->Read(this->GetSize(), 0);
}




File::FileWriter::FileWriter(const char* Filename) {
	this->OpenFile(Filename);
}

void File::FileWriter::OpenFile(const char* Filename) {

	fopen_s(&this->Stream, Filename, "wb");
	if (this->Stream == nullptr) {
		Exceptions::ThrowException<Exceptions::IOError>("Failed to open a file for writing.");
	}
}

void File::FileWriter::Write(const void* Buffer, size_t WriteBytes, long Offset) {

	this->Seek(Offset);
	fwrite(Buffer, 1, WriteBytes, this->Stream);
	this->Seek(0);
}

#endif