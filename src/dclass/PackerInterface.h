// Filename: PackerInterface.h
// Created by: drose (15 Jun, 2004)
//
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//

#pragma once
#include "dcbase.h"
#include "SubatomicType.h"
namespace dclass   // open namespace dclass
{

class File;
class Field;
class SimpleParameter;
class SwitchParameter;
class ClassParameter;
class ArrayParameter;
class AtomicField;
class MolecularField;
class PackData;
class PackerCatalog;

BEGIN_PUBLISH
// This enumerated type is returned by get_pack_type() and represents
// the best choice for a subsequent call to pack_*() or unpack_*().
enum PackType
{
    // This one should never be returned in a normal situation.
    PT_invalid,

    // These PackTypes are all fundamental types, and should be packed
    // (or unpacked) with the corresponding call to pack_double(),
    // pack_int(), etc.  PT_blob is the same as PT_string, but implies
    // that the string contains binary data.
    PT_double,
    PT_int,
    PT_uint,
    PT_int64,
    PT_uint64,
    PT_string,
    PT_blob,

    // The remaining PackTypes imply a need to call push() and pop().
    // They are all variants on the same thing: a list of nested fields,
    // but the PackType provides a bit of a semantic context.
    PT_array,
    PT_field,
    PT_class,
    PT_switch,
};
END_PUBLISH

////////////////////////////////////////////////////////////////////
//       Class : PackerInterface
// Description : This defines the internal interface for packing
//               values into a Field.  The various different 
//               objects inherit from this.
//
//               Normally these methods are called only by the
//               Packer object; the user wouldn't normally call
//               these directly.
////////////////////////////////////////////////////////////////////
class EXPCL_DIRECT PackerInterface
{
	public:
		PackerInterface(const string &name = string());
		PackerInterface(const PackerInterface &copy);
		virtual ~PackerInterface();

	PUBLISHED:
		inline const string &get_name() const;
		int find_seek_index(const string &name) const;

		virtual Field *as_field();
		virtual const Field *as_field() const;
		virtual SwitchParameter *as_switch_parameter();
		virtual const SwitchParameter *as_switch_parameter() const;
		virtual ClassParameter *as_class_parameter();
		virtual const ClassParameter *as_class_parameter() const;

		inline bool check_match(const PackerInterface *other) const;
		bool check_match(const string &description, File *dcfile = NULL) const;

	public:
		virtual void set_name(const string &name);
		inline bool has_fixed_byte_size() const;
		inline size_t get_fixed_byte_size() const;
		inline bool has_fixed_structure() const;
		inline bool has_range_limits() const;
		inline size_t get_num_length_bytes() const;

		inline bool has_nested_fields() const;
		inline int get_num_nested_fields() const;
		virtual int calc_num_nested_fields(size_t length_bytes) const;
		virtual PackerInterface *get_nested_field(int n) const;

		virtual bool validate_num_nested_fields(int num_nested_fields) const;

		inline PackType get_pack_type() const;

		virtual void pack_double(PackData &pack_data, double value,
		                         bool &pack_error, bool &range_error) const;
		virtual void pack_int(PackData &pack_data, int value,
		                      bool &pack_error, bool &range_error) const;
		virtual void pack_uint(PackData &pack_data, unsigned int value,
		                       bool &pack_error, bool &range_error) const;
		virtual void pack_int64(PackData &pack_data, int64_t value,
		                        bool &pack_error, bool &range_error) const;
		virtual void pack_uint64(PackData &pack_data, uint64_t value,
		                         bool &pack_error, bool &range_error) const;
		virtual void pack_string(PackData &pack_data, const string &value,
		                         bool &pack_error, bool &range_error) const;
		virtual bool pack_default_value(PackData &pack_data, bool &pack_error) const;

		virtual void unpack_double(const char *data, size_t length, size_t &p,
		                           double &value, bool &pack_error, bool &range_error) const;
		virtual void unpack_int(const char *data, size_t length, size_t &p,
		                        int &value, bool &pack_error, bool &range_error) const;
		virtual void unpack_uint(const char *data, size_t length, size_t &p,
		                         unsigned int &value, bool &pack_error, bool &range_error) const;
		virtual void unpack_int64(const char *data, size_t length, size_t &p,
		                          int64_t &value, bool &pack_error, bool &range_error) const;
		virtual void unpack_uint64(const char *data, size_t length, size_t &p,
		                           uint64_t &value, bool &pack_error, bool &range_error) const;
		virtual void unpack_string(const char *data, size_t length, size_t &p,
		                           string &value, bool &pack_error, bool &range_error) const;
		virtual bool unpack_validate(const char *data, size_t length, size_t &p,
		                             bool &pack_error, bool &range_error) const;
		virtual bool unpack_skip(const char *data, size_t length, size_t &p,
		                         bool &pack_error) const;

		// These are the low-level interfaces for packing and unpacking
		// numbers from a buffer.  You're responsible for making sure the
		// buffer has enough room, and for incrementing the pointer.
		inline static void do_pack_int8(char *buffer, int8_t value);
		inline static void do_pack_int16(char *buffer, int16_t value);
		inline static void do_pack_int32(char *buffer, int32_t value);
		inline static void do_pack_int64(char *buffer, int64_t value);
		inline static void do_pack_uint8(char *buffer, uint8_t value);
		inline static void do_pack_uint16(char *buffer, uint16_t value);
		inline static void do_pack_uint32(char *buffer, uint32_t value);
		inline static void do_pack_uint64(char *buffer, uint64_t value);
		inline static void do_pack_float64(char *buffer, double value);
		inline static void do_pack_length_tag(char* buffer, length_tag_t value);

		inline static int8_t do_unpack_int8(const char *buffer);
		inline static int16_t do_unpack_int16(const char *buffer);
		inline static int32_t do_unpack_int32(const char *buffer);
		inline static int64_t do_unpack_int64(const char *buffer);
		inline static uint8_t do_unpack_uint8(const char *buffer);
		inline static uint16_t do_unpack_uint16(const char *buffer);
		inline static uint32_t do_unpack_uint32(const char *buffer);
		inline static uint64_t do_unpack_uint64(const char *buffer);
		inline static double do_unpack_float64(const char *buffer);
		inline static length_tag_t do_unpack_length_tag(const char* buffer);

		inline static void validate_int_limits(int value, int num_bits,
		                                       bool &range_error);
		inline static void validate_int64_limits(int64_t value, int num_bits,
		        bool &range_error);
		inline static void validate_uint_limits(unsigned int value, int num_bits,
		                                        bool &range_error);
		inline static void validate_uint64_limits(uint64_t value, int num_bits,
		        bool &range_error);

		const PackerCatalog *get_catalog() const;

	protected:
		virtual bool do_check_match(const PackerInterface *other) const = 0;

	public:
		// These are declared public just so the derived classes can call
		// them easily.  They're not intended to be called directly.

		virtual bool do_check_match_simple_parameter(const SimpleParameter *other) const;
		virtual bool do_check_match_class_parameter(const ClassParameter *other) const;
		virtual bool do_check_match_switch_parameter(const SwitchParameter *other) const;
		virtual bool do_check_match_array_parameter(const ArrayParameter *other) const;
		virtual bool do_check_match_atomic_field(const AtomicField *other) const;
		virtual bool do_check_match_molecular_field(const MolecularField *other) const;

	private:
		void make_catalog();

	protected:
		string _name;
		bool _has_fixed_byte_size;
		size_t _fixed_byte_size;
		bool _has_fixed_structure;
		bool _has_range_limits;
		size_t _num_length_bytes;
		bool _has_nested_fields;
		int _num_nested_fields;
		PackType _pack_type;

	private:
		PackerCatalog *_catalog;
};


} // close namespace dclass

#include "PackerInterface.ipp"
