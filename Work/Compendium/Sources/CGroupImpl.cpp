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

#include "Compendium.hpp"

using Compendium::ENErrorCodes;

using Compendium::CUnit;
using Compendium::CGroup;

CGroup::CGroup() noexcept:
  VId( U"" ) {}

CGroup::CGroup( const CGroup &_Copy ) noexcept:
  VId( _Copy.FGetId() ),
  VGroups( _Copy.FGetGroups() ), VUnits( _Copy.FGetUnits() ) {}

CGroup::CGroup( const u32string &_Id ) :
  VId( _Id ) {}

CGroup::CGroup( const u32string &_Id,
                const vector<CGroup *> &_Groups, const vector<CUnit *> &_Units ) :
  VId( _Id ), VGroups( _Groups ), VUnits( _Units ) {}

CGroup::~CGroup() {
  FClearId();

  VGroups.clear();
  VUnits.clear();
}

//Units

CUnit *CGroup::FGetUnit( const uint32_t _Index ) const {
  if( _Index > VUnits.size() )
    return nullptr;

  return VUnits [ _Index ];
}

CUnit *CGroup::FGetUnit( const u32string &_Id, const bool _Deep ) const {
  for( CUnit *VUnit : VUnits ) {
    if( VUnit->FGetId() == _Id )
      return VUnit;
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CUnit *VGetUnit = VGroup->FGetUnit( _Id, _Deep );

      if( VGetUnit != nullptr )
        return VGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::FGetUnit( const CUnit *_Unit, const bool _Deep ) const {
  if( _Unit == nullptr )
    return nullptr;

  for( CUnit *VUnit : VUnits ) {
    if( VUnit->FGetId() == _Unit->FGetId() &&
        VUnit->FGetValue() == _Unit->FGetValue() )
      return VUnit;
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CUnit *VGetUnit = VGroup->FGetUnit( _Unit, _Deep );

      if( VGetUnit != nullptr )
        return VGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::FRemoveUnit( const u32string &_Id, const bool _Deep ) {
  for( uint32_t c = 0; c < VUnits.size(); c++ ) {
    if( VUnits [ c ]->FGetId() == _Id ) {
      CUnit *VGetUnit = VUnits [ c ];

      VUnits.erase( VUnits.begin() + static_cast< vector<CUnit *>::difference_type >( c ) );

      return VGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CUnit *VGetUnit = VGroup->FRemoveUnit( _Id, _Deep );

      if( VGetUnit != nullptr )
        return VGetUnit;
    }
  }

  return nullptr;
}

ENErrorCodes CGroup::FAddUnit( CUnit *_Unit ) {
  if( _Unit == nullptr )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  VUnits.push_back( _Unit );

  return ENErrorCodes::EC_OK;
}

CUnit *CGroup::FRemoveUnit( const uint32_t _Index ) {
  if( _Index >= VUnits.size() )
    return nullptr;

  CUnit *VGetUnit = VUnits [ _Index ];

  VUnits.erase( VUnits.begin() + static_cast< vector<CUnit *>::difference_type >( _Index ) );

  return VGetUnit;
}

ENErrorCodes CGroup::FInsertUnit( CUnit *_Unit, const uint32_t _Index ) {
  if( _Unit == nullptr || _Index >= VUnits.size() )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  VUnits.insert( VUnits.begin() + static_cast< vector<CUnit *>::difference_type >( _Index ), _Unit );

  return ENErrorCodes::EC_OK;
}

int64_t CGroup::FGetUnitIndex( const u32string &_Id ) const {
  for( uint32_t c = 0; c < VUnits.size(); c++ ) {
    if( VUnits [ c ]->FGetId() == _Id )
      return static_cast< int64_t >( c );
  }

  return static_cast< int64_t >( ENErrorCodes::EC_UNIT_NOT_FOUND );
}

CUnit *CGroup::FChangeUnit( const uint32_t _Index, CUnit *_Substitute ) {
  if( _Index >= VUnits.size() || _Substitute == nullptr )
    return nullptr;

  CUnit *VGetUnit = VUnits [ _Index ];

  VUnits [ _Index ] = _Substitute;

  return VGetUnit;
}

CUnit *CGroup::FChangeUnit( const u32string &_Id, CUnit *_Substitute, const bool _Deep ) {
  if( _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VUnits.size(); c++ ) {
    if( VUnits [ c ]->FGetId() == _Id ) {
      CUnit *VGetUnit = VUnits [ c ];

      VUnits [ c ] = _Substitute;

      return VGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CUnit *VGetUnit = VGroup->FChangeUnit( _Id, _Substitute, _Deep );

      if( VGetUnit != nullptr )
        return VGetUnit;
    }
  }

  return nullptr;
}

//Groups

CGroup *CGroup::FGetGroup( const uint32_t _Index ) const {
  if( _Index >= VGroups.size() )
    return nullptr;

  return VGroups [ _Index ];
}

CGroup *CGroup::FGetGroup( const u32string &_Id, const bool _Deep ) const {
  if( _Id.empty() )
    return nullptr;

  for( CGroup *VGroup : VGroups ) {
    if( VGroup->FGetId() == _Id )
      return VGroup;
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FGetGroup( _Id, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FGetGroup( const CUnit *_Unit, const bool _Deep ) const {
  if( _Unit == nullptr )
    return nullptr;

  for( CGroup *VGroup : VGroups ) {
    if( VGroup->FGetUnit( _Unit, _Deep ) != nullptr )
      return VGroup;
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FGetGroup( _Unit, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FGetGroup( const vector<CUnit *> &_Units, const bool _Deep ) const {
  if( _Units.empty() )
    return nullptr;

  for( CGroup *VGroup : VGroups ) {
    for( const CUnit *VUnit : _Units ) {
      if( VGroup->FGetUnit( VUnit, _Deep ) != nullptr )
        return VGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CUnit *VUnit : _Units ) {
        CGroup *VGetGroup = VGroup->FGetGroup( VUnit, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::FGetGroup( const CGroup *_Group, const bool _Deep ) const {
  if( _Group == nullptr )
    return nullptr;

  for( CGroup *VGroup : VGroups ) {
    if( VGroup->FGetGroup( _Group, _Deep ) != nullptr )
      return VGroup;
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FGetGroup( _Group, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FGetGroup( const vector<CGroup *> &_Groups, const bool _Deep ) const {
  if( _Groups.empty() )
    return nullptr;

  for( CGroup *VGroup : VGroups ) {
    for( const CGroup *VInnerGroup : _Groups ) {
      if( VGroup->FGetGroup( VInnerGroup, _Deep ) != nullptr )
        return VGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CGroup *VInnerGroup : _Groups ) {
        CGroup *VGetGroup = VGroup->FGetGroup( VInnerGroup, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}

ENErrorCodes CGroup::FAddGroup( CGroup *_Group ) {
  if( _Group == nullptr )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  VGroups.push_back( _Group );

  return ENErrorCodes::EC_OK;
}

CGroup *CGroup::FRemoveGroup( const uint32_t _Index ) {
  if( _Index >= VGroups.size() )
    return nullptr;

  CGroup *VGetGroup = VGroups [ _Index ];

  VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( _Index ) );

  return VGetGroup;
}

CGroup *CGroup::FRemoveGroup( const u32string &_Id, const bool _Deep ) {
  if( _Id.empty() )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetId() == _Id ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FRemoveGroup( _Id, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FRemoveGroup( const CUnit *_Unit, const bool _Deep ) {
  if( _Unit == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetUnit( _Unit, _Deep ) != nullptr ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FRemoveGroup( _Unit, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FRemoveGroup( const vector<CUnit *> &_Units, const bool _Deep ) {
  if( _Units.empty() )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CUnit *VUnit : _Units ) {
      if( VGroups [ c ]->FGetUnit( VUnit, _Deep ) != nullptr ) {
        CGroup *VGetGroup = VGroups [ c ];

        VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

        return VGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CUnit *VUnit : _Units ) {
        CGroup *VGetGroup = VGroup->FRemoveGroup( VUnit, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::FRemoveGroup( const CGroup *_Group, const bool _Deep ) {
  if( _Group == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetGroup( _Group, _Deep ) != nullptr ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FRemoveGroup( _Group, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FRemoveGroup( const vector<CGroup *> &_Groups, const bool _Deep ) {
  if( _Groups.empty() )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CGroup *VGroup : _Groups ) {
      if( VGroups [ c ]->FGetGroup( VGroup, _Deep ) != nullptr ) {
        CGroup *VGetGroup = VGroups [ c ];

        VGroups.erase( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

        return VGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CGroup *VInnerGroup : _Groups ) {
        CGroup *VGetGroup = VGroup->FRemoveGroup( VInnerGroup, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}

ENErrorCodes CGroup::FInsertGroup( CGroup *&_Group, const uint32_t _Index ) {
  if( _Group == nullptr || _Index >= VGroups.size() )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  VGroups.insert( VGroups.begin() + static_cast< vector<CGroup *>::difference_type >( _Index ), _Group );

  return ENErrorCodes::EC_OK;
}

int64_t CGroup::FGetGroupIndex( const u32string &_Id ) const {
  if( _Id.empty() )
    return static_cast< int64_t >( ENErrorCodes::EC_INVALID_ARGUMENT );

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetId() == _Id )
      return static_cast< int64_t >( c );
  }

  return static_cast< int64_t >( ENErrorCodes::EC_GROUP_NOT_FOUND );
}

int64_t CGroup::FGetGroupIndex( const CUnit *_Unit ) const {
  if( _Unit == nullptr )
    return static_cast< int64_t >( ENErrorCodes::EC_INVALID_ARGUMENT );

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetUnit( _Unit ) != nullptr )
      return static_cast< int64_t >( c );
  }

  return static_cast< int64_t >( ENErrorCodes::EC_GROUP_NOT_FOUND );
}

int64_t CGroup::FGetGroupIndex( const vector<CUnit *> &_Units ) const {
  if( _Units.empty() )
    return static_cast< int64_t >( ENErrorCodes::EC_INVALID_ARGUMENT );

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CUnit *VUnit : _Units ) {
      if( VGroups [ c ]->FGetUnit( VUnit ) != nullptr )
        return static_cast< int64_t >( c );
    }
  }

  return static_cast< int64_t >( ENErrorCodes::EC_GROUP_NOT_FOUND );
}

int64_t CGroup::FGetGroupIndex( const CGroup *_Group ) const {
  if( _Group == nullptr )
    return static_cast< int64_t >( ENErrorCodes::EC_INVALID_ARGUMENT );

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetGroup( _Group ) != nullptr )
      return static_cast< int64_t >( c );
  }

  return static_cast< int64_t >( ENErrorCodes::EC_GROUP_NOT_FOUND );
}

int64_t CGroup::FGetGroupIndex( const vector<CGroup *> &_Groups ) const {
  if( _Groups.empty() )
    return static_cast< int64_t >( ENErrorCodes::EC_INVALID_ARGUMENT );

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CGroup *VGroup : _Groups ) {
      if( VGroups [ c ]->FGetGroup( VGroup ) != nullptr )
        return static_cast< int64_t >( c );
    }
  }

  return static_cast< int64_t >( ENErrorCodes::EC_GROUP_NOT_FOUND );
}

CGroup *CGroup::FChangeGroup( const uint32_t _Index, CGroup *_Substitute ) {
  if( _Index >= VGroups.size() || _Substitute == nullptr )
    return nullptr;

  CGroup *VGetGroup = VGroups [ _Index ];

  VGroups [ _Index ] = _Substitute;

  return VGetGroup;
}

CGroup *CGroup::FChangeGroup( const u32string &_Id, CGroup *_Substitute, const bool _Deep ) {
  if( _Id.empty() || _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetId() == _Id ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups [ c ] = _Substitute;

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FChangeGroup( _Id, _Substitute, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FChangeGroup( const CUnit *_Unit, CGroup *_Substitute, const bool _Deep ) {
  if( _Unit == nullptr || _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetUnit( _Unit ) != nullptr ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups [ c ] = _Substitute;

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FChangeGroup( _Unit, _Substitute, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FChangeGroup( const vector<CUnit *> &_Units, CGroup *_Substitute, const bool _Deep ) {
  if( _Units.empty() || _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CUnit *VUnit : _Units ) {
      if( VGroups [ c ]->FGetUnit( VUnit ) != nullptr ) {
        CGroup *VGetGroup = VGroups [ c ];

        VGroups [ c ] = _Substitute;

        return VGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CUnit *VUnit : VUnits ) {
        CGroup *VGetGroup = VGroup->FChangeGroup( VUnit, _Substitute, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::FChangeGroup( const CGroup *_Group, CGroup *_Substitute, const bool _Deep ) {
  if( _Group == nullptr || _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    if( VGroups [ c ]->FGetGroup( _Group ) != nullptr ) {
      CGroup *VGetGroup = VGroups [ c ];

      VGroups [ c ] = _Substitute;

      return VGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      CGroup *VGetGroup = VGroup->FChangeGroup( _Group, _Substitute, _Deep );

      if( VGetGroup != nullptr )
        return VGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::FChangeGroup( const vector<CGroup *> &_Groups, CGroup *_Substitute, const bool _Deep ) {
  if( _Groups.empty() || _Substitute == nullptr )
    return nullptr;

  for( uint32_t c = 0; c < VGroups.size(); c++ ) {
    for( const CGroup *VGroup : _Groups ) {
      if( VGroups [ c ]->FGetGroup( VGroup ) != nullptr ) {
        CGroup *VGetGroup = VGroups [ c ];

        VGroups [ c ] = _Substitute;

        return VGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *VGroup : VGroups ) {
      for( const CGroup *VInnerGroup : VGroups ) {
        CGroup *VGetGroup = VGroup->FChangeGroup( VInnerGroup, _Substitute, _Deep );

        if( VGetGroup != nullptr )
          return VGetGroup;
      }
    }
  }

  return nullptr;
}
