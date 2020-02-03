/*
© 2019, Serge Page.

This license is hereby grants to any person who obtained a copy of this product or the product source files the next rights to:

- Use a compiled version of this product at no cost, without any restrictions, in non-commercial and commercial purposes
- Do reverse-engineering of this product in non-commercial purposes only
- Use source codes of this product at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
- Copy and distribute without any fee
- Copy of the original repository and / or create own derivative product for non-commercial,  academic and / or scientific purposes only
- Link the product source code with an another product source code which licensed under any of Dark Orb licenses or one of these licenses:
  - MIT License
  - Microsoft Public License
  - Beerware License
  - Academic Free License
  - WTFPL
  - Unlicense
  - Original BSD license
  - Modified BSD License
  - Simplified BSD License
  - Zero Clause BSD
- Link the product source code with an another product source code if between them no any patent collision

This license is require to:

- Keep the full license text without any changes
- The license text must be included once in a file called 'License' which placed in the root directory of the product and in all source files of the product

This license is deny to:

- Change license of the derivative product
- Use the product’s author name and name of any contributor of this product for advertising derivative software without legally certified permission
- Resell this product
- Use the product or the product source code for any purpose which refers to any government of any country

The product is an original source codes and original compiled files which made by the original author and provided only under the grants and restrictions of this license. All damages which can be happen after and while using the product will not be compensate.
*/

#pragma once

#include "CHeader.hpp"

namespace Compendium {
  enum class ENErrorCodes: int64_t {
    EC_INVALID_ARGUMENT = numeric_limits<int64_t>::lowest(),
    EC_INVALID_PATH,
    EC_INVALID_READ,
    EC_INVALID_PARSE,
    EC_INVALID_START_INDEX,

    EC_UNIT_NOT_FOUND,
    EC_GROUP_NOT_FOUND,
    EC_NOT_FOUND_KEYWORD,

    EC_NOT_COMPENDIUM_FILE,

    EC_EMPTY_FILE,

    EC_OK = 0
  };

  class CUnit {
    public:
    explicit CUnit() noexcept;
    explicit CUnit( const CUnit &_Copy ) noexcept;
    explicit CUnit( const u32string &_Id, const u32string &_Value );
    ~CUnit();

    inline u32string FGetId() const noexcept {
      return VId;
    }

    void FSetId( const u32string &_Id ) noexcept {
      VId = _Id;
    }

    inline u32string FGetValue() const noexcept {
      return VValue;
    }

    inline void FSetValue( const char32_t *_Value ) noexcept {
      VValue = _Value;
    }

    private:
    u32string VId;
    u32string VValue;

    inline void FClearId() {
      VId.clear();
    }

    inline void FClearValue() {
      VValue.clear();
    }
  };

  class CGroup {
    public:
    explicit CGroup() noexcept;
    explicit CGroup( const CGroup &_Copy ) noexcept;
    explicit CGroup( const u32string &_Id );
    explicit CGroup( const u32string &_Id,
                     const vector<CGroup *> &_Groups, const vector<CUnit *> &_Units );
    ~CGroup();

    inline u32string FGetId() const noexcept {
      return VId;
    }

    inline void FSetId( const u32string &_Id ) {
      VId = _Id;
    }

    inline vector<CGroup *> FGetGroups() const noexcept {
      return VGroups;
    }

    inline void FSetGroups( vector<CGroup *> &_Groups ) noexcept {
      VGroups = _Groups;
    }

    inline void FClearGroups() noexcept {
      VGroups.clear();
    }

    inline vector<CUnit *> FGetUnits() const noexcept {
      return VUnits;
    }

    inline void FSetUnits( vector<CUnit *> &_Units ) noexcept {
      VUnits = _Units;
    }

    inline void FClearUnits() noexcept {
      VUnits.clear();
    }

    //Units

    CUnit *FGetUnit( const uint32_t _Index ) const;
    CUnit *FGetUnit( const u32string &_Id, const bool _Deep = false ) const;
    CUnit *FGetUnit( const CUnit *&_Unit, const bool _Deep = false ) const;

    ENErrorCodes FAddUnit( CUnit *&_Unit );

    CUnit *FRemoveUnit( const uint32_t _Index );
    CUnit *FRemoveUnit( const u32string &_Id, const bool _Deep = false );

    ENErrorCodes FInsertUnit( CUnit *&_Unit, const uint32_t _Index );

    int64_t FGetUnitIndex( const u32string &_Id ) const;

    CUnit *FChangeUnit( const uint32_t _Index, CUnit *&_Substitute );
    CUnit *FChangeUnit( const u32string &_Id, CUnit *&_Substitute, const bool _Deep = false );

    //Groups

    CGroup *FGetGroup( const uint32_t _Index ) const;
    CGroup *FGetGroup( const u32string &_Id, const bool _Deep = false ) const;
    CGroup *FGetGroup( const CUnit *&_Unit, const bool _Deep = false ) const;
    CGroup *FGetGroup( const vector<CUnit *> &_Units, const bool _Deep = false ) const;
    CGroup *FGetGroup( const CGroup *&_Group, const bool _Deep = false ) const;
    CGroup *FGetGroup( const vector<CGroup *> &_Groups, const bool _Deep = false ) const;

    ENErrorCodes FAddGroup( CGroup *&_Group );

    CGroup *FRemoveGroup( const uint32_t _Index );
    CGroup *FRemoveGroup( const u32string &_Id, const bool _Deep = false );
    CGroup *FRemoveGroup( const CUnit *&_Unit, const bool _Deep = false );
    CGroup *FRemoveGroup( const vector<CUnit *> &_Units, const bool _Deep = false );
    CGroup *FRemoveGroup( const CGroup *&_Group, const bool _Deep = false );
    CGroup *FRemoveGroup( const vector<CGroup *> &_Groups, const bool _Deep = false );

    ENErrorCodes FInsertGroup( CGroup *&_Group, const uint32_t _Index );

    int64_t FGetGroupIndex( const u32string &_Id ) const;
    int64_t FGetGroupIndex( const CUnit *&_Unit ) const;
    int64_t FGetGroupIndex( const vector<CUnit *> &_Units ) const;
    int64_t FGetGroupIndex( const CGroup *&_Group ) const;
    int64_t FGetGroupIndex( const vector<CGroup *> &_Groups ) const;

    CGroup *FChangeGroup( const uint32_t _Index, CGroup *&_Substitute );
    CGroup *FChangeGroup( const u32string &_Id, CGroup *&_Substitute, const bool _Deep = false );
    CGroup *FChangeGroup( const CUnit *&_Unit, CGroup *&_Substitute, const bool _Deep = false );
    CGroup *FChangeGroup( const vector<CUnit *> &_Units, CGroup *&_Substitute, const bool _Deep = false );
    CGroup *FChangeGroup( const CGroup *&_Group, CGroup *&_Substitute, const bool _Deep = false );
    CGroup *FChangeGroup( const vector<CGroup *> &_Groups, CGroup *&_Substitute, const bool _Deep = false );

    private:
    u32string VId;

    vector<CGroup *> VGroups;
    vector<CUnit *> VUnits;

    inline void FClearId() {
      VId.clear();
    }
  };

  class CConfigurator {
    public:
    explicit CConfigurator() noexcept;
    ~CConfigurator();

    inline u32string FGetBuffer() const noexcept {
      return VBuffer;
    }

    ENErrorCodes FLoadBufferFromFile( const u32string &_Path );
    ENErrorCodes FLoadBufferFromBuffer( const u32string &_Buffer );

    ENErrorCodes FLoadConfigurationFromFile( const u32string &_Path );
    ENErrorCodes FLoadConfigurationFromBuffer( const u32string &_Buffer, const uint32_t _StartIndex );

    ENErrorCodes FSaveConfigurationToFile( const u32string &_Path );
    ENErrorCodes FSaveConfigurationToBuffer( u32string &_Path );

    inline vector<CGroup *> FGetGroups() const noexcept {
      return VGroups;
    }
    inline void FSetGroups( vector<CGroup *> &_Groups ) noexcept {
      VGroups = _Groups;
    }

    inline void FClearGroups() noexcept {
      VGroups.clear();
    }

    inline vector<CUnit *> FGetUnits() const noexcept {
      return VUnits;
    }
    inline void FSetUnits( vector<CUnit *> &_Units ) noexcept {
      VUnits = _Units;
    }

    inline void FClearUnits() noexcept {
      VUnits.clear();
    }

    private:
    const char32_t *VDefaultExtension = U".compendium-configuration";

    bool FIsCompendiumFile( const u32string &_Path ) const noexcept;

    u32string VBuffer;

    inline void FClearBuffer() {
      VBuffer.clear();
    }

    vector<CUnit *> VUnits;
    vector<CGroup *> VGroups;

    void FSkipComment( uint32_t &_Index ) noexcept;

    enum class ENKeywords: int64_t {
      K_GROUP,
      K_UNIT
    };

    int64_t FGetNextKeyword( uint32_t &_Index ) const noexcept;

    CGroup *FParseGroup( uint32_t &_Index );
    CUnit *FParseUnit( uint32_t &_Index );

    u32string FSerializeGroup( const CGroup *&_Group, const uint32_t _Level ) const;
    u32string FSerializeUnit( const CUnit *&_Unit ) const;
  };
}
