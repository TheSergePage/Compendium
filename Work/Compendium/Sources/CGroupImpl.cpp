/*
© 2019, Dark Orb.

The license version - 1.0

This license is hereby grants to any person who obtained a copy of this software the next rights to:
1. Use and do reverse-engineering of compiled version of this software at no cost, without any restrictions, in non-commercial and commercial purposes
2. Use source codes of this software at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
3. Copy and distribute without any fee
4. Create a copy of the exists repository with this software for non-commercial,  academic and / or scientific purposes only

This license is require to:
1. Keep the full license text without any changes
2. The license text must be included once in a file called 'License' which placed in the root directory of the software and in all source files of the software

This license is deny to:
1. Change license of the derivative software
2. Use the copyright holder name and name of any contributor of this software for advertising derivative software without legally certified permission
3. Sell this software without an author legally certified permission 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Compendium.hpp"

using Compendium::enErrorCode;

using Compendium::CUnit;
using Compendium::CGroup;

CGroup::CGroup() noexcept:
  vId( nullptr ) {}

CGroup::CGroup( const CGroup &_Copy ) noexcept:
  vId( _Copy.fGetId() ),
  vGroups( _Copy.fGetGroups() ), vUnits( _Copy.fGetUnits() ) {}

CGroup::CGroup( const wchar_t *_Id ) noexcept:
  vId( nullptr ) {
  if( _Id != nullptr ) {
    size_t vSourceSize = wcslen( _Id ) + 1;
    vId = new wchar_t [ vSourceSize ];
    wcscpy_s( vId, vSourceSize, _Id );
  }
}

CGroup::CGroup( const wchar_t *_Id,
                vector<CGroup *> _Groups, vector<CUnit *> _Units ) noexcept:
  vId( nullptr ), vGroups( _Groups ), vUnits( _Units ) {
  if( _Id != nullptr ) {
    size_t vSourceSize = wcslen( _Id ) + 1;
    vId = new wchar_t [ wcslen( _Id ) + 1 ];
    wcscpy_s( vId, vSourceSize, _Id );
  }
}

CGroup::~CGroup() {
  fClearId();

  vGroups.clear();
  vUnits.clear();
}

//Units

CUnit *CGroup::fGetUnit( const wstring _Id, const bool _Deep ) const {
  for( CUnit *vUnit : vUnits ) {
    if( vUnit->fGetId() == _Id )
      return vUnit;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fGetUnit( _Id, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fGetUnit( const wchar_t *_Value, const bool _Deep ) const {
  if( _Value == nullptr )
    return nullptr;

  for( CUnit *vUnit : vUnits ) {
    if( wcscmp( vUnit->fGetValue(), _Value ) != 0 )
      return vUnit;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fGetUnit( _Value, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fGetUnit( const CUnit *_Unit, const bool _Deep ) const {
  if( _Unit == nullptr )
    return nullptr;

  for( CUnit *vUnit : vUnits ) {
    if( vUnit->fGetId() == _Unit->fGetId() &&
        wcscmp( vUnit->fGetValue(), _Unit->fGetValue() ) == 0 )
      return vUnit;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fGetUnit( _Unit, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fRemoveUnit( const wstring _Id, const bool _Deep ) {
  for( size_t c = 0; c < vUnits.size(); c++ ) {
    if( vUnits [ c ]->fGetId() == _Id ) {
      CUnit *vGetUnit = vUnits [ c ];

      vUnits.erase( vUnits.begin() + static_cast< vector<CUnit *>::difference_type >( c ) );

      return vGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fRemoveUnit( _Id, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fRemoveUnit( const wchar_t *_Value, const bool _Deep ) {
  if( _Value == nullptr )
    return nullptr;

  for( size_t c = 0; c < vUnits.size(); c++ ) {
    if( wcscmp( vUnits [ c ]->fGetValue(), _Value ) != 0 ) {
      CUnit *vGetUnit = vUnits [ c ];

      vUnits.erase( vUnits.begin() + static_cast< vector<CUnit *>::difference_type >( c ) );

      return vGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fRemoveUnit( _Value, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fChangeUnit( const wstring _Id, CUnit *_Substitute, const bool _Deep ) {
  if( _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vUnits.size(); c++ ) {
    if( wcscmp( vUnits [ c ]->fGetId(), _Id.data() ) == 0 ) {
      CUnit *vGetUnit = vUnits [ c ];

      vUnits [ c ] = _Substitute;

      return vGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fChangeUnit( _Id, _Substitute, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

CUnit *CGroup::fChangeUnit( const wchar_t *_Value, CUnit *_Substitute, const bool _Deep ) {
  if( _Value == nullptr || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vUnits.size(); c++ ) {
    if( wcscmp( vUnits [ c ]->fGetValue(), _Value ) == 0 ) {
      CUnit *vGetUnit = vUnits [ c ];

      vUnits [ c ] = _Substitute;

      return vGetUnit;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CUnit *vGetUnit = vGroup->fChangeUnit( _Value, _Substitute, _Deep );

      if( vGetUnit != nullptr )
        return vGetUnit;
    }
  }

  return nullptr;
}

//Groups

CGroup *CGroup::fGetGroup( const wchar_t *_Id, const bool _Deep ) const {
  if( _Id == nullptr )
    return nullptr;

  for( CGroup *vGroup : vGroups ) {
    if( wcscmp( vGroup->fGetId(), _Id ) == 0 )
      return vGroup;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fGetGroup( _Id, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fGetGroup( const CUnit *_Unit, const bool _Deep ) const {
  if( _Unit == nullptr )
    return nullptr;

  for( CGroup *vGroup : vGroups ) {
    if( vGroup->fGetUnit( _Unit, _Deep ) != nullptr )
      return vGroup;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fGetGroup( _Unit, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fGetGroup( const vector<CUnit *> _Units, const bool _Deep ) const {
  if( _Units.empty() )
    return nullptr;

  for( CGroup *vGroup : vGroups ) {
    for( const CUnit *vUnit : _Units ) {
      if( vGroup->fGetUnit( vUnit, _Deep ) != nullptr )
        return vGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CUnit *vUnit : _Units ) {
        CGroup *vGetGroup = vGroup->fGetGroup( vUnit, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::fGetGroup( const CGroup *_Group, const bool _Deep ) const {
  if( _Group == nullptr )
    return nullptr;

  for( CGroup *vGroup : vGroups ) {
    if( vGroup->fGetGroup( _Group, _Deep ) != nullptr )
      return vGroup;
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fGetGroup( _Group, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fGetGroup( const vector<CGroup *> _Groups, const bool _Deep ) const {
  if( _Groups.empty() )
    return nullptr;

  for( CGroup *vGroup : vGroups ) {
    for( const CGroup *vInnerGroup : _Groups ) {
      if( vGroup->fGetGroup( vInnerGroup, _Deep ) != nullptr )
        return vGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CGroup *vInnerGroup : _Groups ) {
        CGroup *vGetGroup = vGroup->fGetGroup( vInnerGroup, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::fRemoveGroup( const wchar_t *_Id, const bool _Deep ) {
  if( _Id == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( wcscmp( vGroups [ c ]->fGetId(), _Id ) == 0 ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fRemoveGroup( _Id, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fRemoveGroup( const CUnit *_Unit, const bool _Deep ) {
  if( _Unit == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( vGroups [ c ]->fGetUnit( _Unit, _Deep ) != nullptr ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fRemoveGroup( _Unit, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fRemoveGroup( const vector<CUnit *> _Units, const bool _Deep ) {
  if( _Units.empty() )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    for( const CUnit *vUnit : _Units ) {
      if( vGroups [ c ]->fGetUnit( vUnit, _Deep ) != nullptr ) {
        CGroup *vGetGroup = vGroups [ c ];

        vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

        return vGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CUnit *vUnit : _Units ) {
        CGroup *vGetGroup = vGroup->fRemoveGroup( vUnit, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::fRemoveGroup( const CGroup *_Group, const bool _Deep ) {
  if( _Group == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( vGroups [ c ]->fGetGroup( _Group, _Deep ) != nullptr ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fRemoveGroup( _Group, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fRemoveGroup( const vector<CGroup *> _Groups, const bool _Deep ) {
  if( _Groups.empty() )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    for( const CGroup *vGroup : _Groups ) {
      if( vGroups [ c ]->fGetGroup( vGroup, _Deep ) != nullptr ) {
        CGroup *vGetGroup = vGroups [ c ];

        vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( c ) );

        return vGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CGroup *vInnerGroup : _Groups ) {
        CGroup *vGetGroup = vGroup->fRemoveGroup( vInnerGroup, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::fChangeGroup( const wchar_t *_Id, CGroup *_Substitute, const bool _Deep ) {
  if( _Id == nullptr || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( wcscmp( vGroups [ c ]->fGetId(), _Id ) == 0 ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups [ c ] = _Substitute;

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fChangeGroup( _Id, _Substitute, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fChangeGroup( const CUnit *_Unit, CGroup *_Substitute, const bool _Deep ) {
  if( _Unit == nullptr || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( vGroups [ c ]->fGetUnit( _Unit ) != nullptr ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups [ c ] = _Substitute;

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fChangeGroup( _Unit, _Substitute, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fChangeGroup( const vector<CUnit *> _Units, CGroup *_Substitute, const bool _Deep ) {
  if( _Units.empty() || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    for( const CUnit *vUnit : _Units ) {
      if( vGroups [ c ]->fGetUnit( vUnit ) != nullptr ) {
        CGroup *vGetGroup = vGroups [ c ];

        vGroups [ c ] = _Substitute;

        return vGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CUnit *vUnit : vUnits ) {
        CGroup *vGetGroup = vGroup->fChangeGroup( vUnit, _Substitute, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}

CGroup *CGroup::fChangeGroup( const CGroup *_Group, CGroup *_Substitute, const bool _Deep ) {
  if( _Group == nullptr || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    if( vGroups [ c ]->fGetGroup( _Group ) != nullptr ) {
      CGroup *vGetGroup = vGroups [ c ];

      vGroups [ c ] = _Substitute;

      return vGetGroup;
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      CGroup *vGetGroup = vGroup->fChangeGroup( _Group, _Substitute, _Deep );

      if( vGetGroup != nullptr )
        return vGetGroup;
    }
  }

  return nullptr;
}

CGroup *CGroup::fChangeGroup( const vector<CGroup *> _Groups, CGroup *_Substitute, const bool _Deep ) {
  if( _Groups.empty() || _Substitute == nullptr )
    return nullptr;

  for( size_t c = 0; c < vGroups.size(); c++ ) {
    for( const CGroup *vGroup : _Groups ) {
      if( vGroups [ c ]->fGetGroup( vGroup ) != nullptr ) {
        CGroup *vGetGroup = vGroups [ c ];

        vGroups [ c ] = _Substitute;

        return vGetGroup;
      }
    }
  }

  if( _Deep ) {
    for( CGroup *vGroup : vGroups ) {
      for( const CGroup *vInnerGroup : vGroups ) {
        CGroup *vGetGroup = vGroup->fChangeGroup( vInnerGroup, _Substitute, _Deep );

        if( vGetGroup != nullptr )
          return vGetGroup;
      }
    }
  }

  return nullptr;
}
