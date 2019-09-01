/*
© 2019, Dark Orb.

The license version - 1.0

This license is hereby grants to any person who obtained a copy of this software the next rights to:
1. Use and do reverse-engineering of compiled version of this software at no cost, without any restrictions, in non-commercial and commercial purposes
2. Use source codes of this software at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
3. Copy and distribute without any fee
4. Create a copy of the exists repository and / or create own derivative software for non-commercial,  academic and / or scientific purposes only

This license is require to:
1. Keep the full license text without any changes
2. The license text must be included once in a file called 'License' which placed in the root directory of the software

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

#pragma once

#include "CHeader.hpp"

namespace Compendium {
  typedef enum {
    EC_INVALID_ARGUMENT = numeric_limits<long>::lowest(),
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
  } enErrorCode;

  class CUnit {
    public:
    explicit CUnit() noexcept;
    explicit CUnit( const CUnit &_Copy ) noexcept;
    explicit CUnit( const wchar_t *_Id, const wchar_t *_Value ) noexcept;
    ~CUnit();

    inline wchar_t *fGetId() const noexcept {
      return vId;
    }

    inline void fSetId( const wchar_t *_Id ) noexcept {
      fClearId();

      size_t vSourceSize = wcslen( _Id ) + 1;

      vId = new wchar_t [ vSourceSize ];
      wcscpy_s( vId, vSourceSize, _Id );
    }

    inline wchar_t *fGetValue() const noexcept {
      return vValue;
    }

    inline void fSetValue( const wchar_t *_Value ) noexcept {
      fClearValue();

      size_t vSourceSize = wcslen( _Value ) + 1;

      vValue = new wchar_t [ vSourceSize ];
      wcscpy_s( vValue, vSourceSize, _Value );
    }

    private:
    wchar_t *vId;
    wchar_t *vValue;

    inline void fClearId() {
      if( vId != nullptr ) {
        delete[] vId;
        vId = nullptr;
      }
    }

    inline void fClearValue() {
      if( vValue != nullptr ) {
        delete[] vValue;
        vValue = nullptr;
      }
    }
  };

  class CGroup {
    public:
    explicit CGroup() noexcept;
    explicit CGroup( const CGroup &_Copy ) noexcept;
    explicit CGroup( const wchar_t *_Id ) noexcept;
    explicit CGroup( const wchar_t *_Id,
                     vector<CGroup *> _Groups, vector<CUnit *> _Units ) noexcept;
    ~CGroup();

    inline wchar_t *fGetId() const noexcept { return vId; }

    inline void fSetId( const wchar_t *_Id ) noexcept {
      fClearId();

      size_t vSourceSize = wcslen( _Id ) + 1;

      vId = new wchar_t [ vSourceSize ];
      wcscpy_s( vId, vSourceSize, _Id );
    }

    inline vector<CGroup *> fGetGroups() const { return vGroups; }

    inline void fSetGroups( vector<CGroup *> _Groups ) noexcept { vGroups = _Groups; }

    inline void fClearGroups() noexcept { vGroups.clear(); }

    inline vector<CUnit *> fGetUnits() const { return vUnits; }

    inline void fSetUnits( vector<CUnit *> _Units ) noexcept { vUnits = _Units; }

    inline void fClearUnits() noexcept { vUnits.clear(); }

    //Units

    inline CUnit *fGetUnit( const size_t _Index ) const {
      if( _Index > vUnits.size() )
        return nullptr;

      return vUnits [ _Index ];
    }

    CUnit *fGetUnit( const wstring _Id, const bool _Deep = false ) const;
    CUnit *fGetUnit( const wchar_t *_Value, const bool _Deep = false ) const;
    CUnit *fGetUnit( const CUnit *_Unit, const bool _Deep = false ) const;

    inline enErrorCode fAddUnit( CUnit *_Unit ) {
      if( _Unit == nullptr )
        return EC_INVALID_ARGUMENT;

      vUnits.push_back( _Unit );

      return EC_OK;
    }

    inline CUnit *fRemoveUnit( const size_t _Index ) {
      if( _Index >= vUnits.size() )
        return nullptr;

      CUnit *vGetUnit = vUnits [ _Index ];

      vUnits.erase( vUnits.begin() + static_cast< vector<CUnit *>::difference_type >( _Index ) );

      return vGetUnit;
    }

    CUnit *fRemoveUnit( const wstring _Id, const bool _Deep = false );
    CUnit *fRemoveUnit( const wchar_t *_Value, const bool _Deep = false );

    inline enErrorCode fInsertUnit( CUnit *_Unit, const size_t _Index ) {
      if( _Unit == nullptr || _Index >= vUnits.size() )
        return EC_INVALID_ARGUMENT;

      vUnits.insert( vUnits.begin() + static_cast< vector<CUnit *>::difference_type >( _Index ), _Unit );

      return EC_OK;
    }

    inline long fGetUnitIndex( const wstring _Id ) const {
      for( size_t c = 0; c < vUnits.size(); c++ ) {
        if( wcscmp( vUnits [ c ]->fGetId(), _Id.data() ) == 0 )
          return static_cast< long >( c );
      }

      return EC_UNIT_NOT_FOUND;
    }

    inline long fGetUnitIndex( const wchar_t *_Value ) const {
      if( _Value == nullptr )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vUnits.size(); c++ ) {
        if( wcscmp( vUnits [ c ]->fGetValue(), _Value ) == 0 )
          return static_cast< long >( c );
      }

      return EC_UNIT_NOT_FOUND;
    }

    inline CUnit *fChangeUnit( const size_t _Index, CUnit *_Substitute ) {
      if( _Index >= vUnits.size() || _Substitute == nullptr )
        return nullptr;

      CUnit *vGetUnit = vUnits [ _Index ];

      vUnits [ _Index ] = _Substitute;

      return vGetUnit;
    }

    CUnit *fChangeUnit( const wstring _Id, CUnit *_Substitute, const bool _Deep = false );
    CUnit *fChangeUnit( const wchar_t *_Value, CUnit *_Substitute, const bool _Deep = false );

    //Groups

    inline CGroup *fGetGroup( const size_t _Index ) const {
      if( _Index >= vGroups.size() )
        return nullptr;

      return vGroups [ _Index ];
    }

    CGroup *fGetGroup( const wchar_t *_Id, const bool _Deep = false ) const;
    CGroup *fGetGroup( const CUnit *_Unit, const bool _Deep = false ) const;
    CGroup *fGetGroup( const vector<CUnit *> _Units, const bool _Deep = false ) const;
    CGroup *fGetGroup( const CGroup *_Group, const bool _Deep = false ) const;
    CGroup *fGetGroup( const vector<CGroup *> _Groups, const bool _Deep = false ) const;

    inline enErrorCode fAddGroup( CGroup *_Group ) {
      if( _Group == nullptr )
        return EC_INVALID_ARGUMENT;

      vGroups.push_back( _Group );

      return EC_OK;
    }

    inline CGroup *fRemoveGroup( const size_t _Index ) {
      if( _Index >= vGroups.size() )
        return nullptr;

      CGroup *vGetGroup = vGroups [ _Index ];

      vGroups.erase( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( _Index ) );

      return vGetGroup;
    }

    CGroup *fRemoveGroup( const wchar_t *_Id, const bool _Deep = false );
    CGroup *fRemoveGroup( const CUnit *_Unit, const bool _Deep = false );
    CGroup *fRemoveGroup( const vector<CUnit *> _Units, const bool _Deep = false );
    CGroup *fRemoveGroup( const CGroup *_Group, const bool _Deep = false );
    CGroup *fRemoveGroup( const vector<CGroup *> _Groups, const bool _Deep = false );

    inline enErrorCode fInsertGroup( CGroup *_Group, const size_t _Index ) {
      if( _Group == nullptr || _Index >= vGroups.size() )
        return EC_INVALID_ARGUMENT;

      vGroups.insert( vGroups.begin() + static_cast< vector<CGroup *>::difference_type >( _Index ), _Group );

      return EC_OK;
    }

    inline long fGetGroupIndex( const wchar_t *_Id ) const {
      if( _Id == nullptr )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vGroups.size(); c++ ) {
        if( wcscmp( vGroups [ c ]->fGetId(), _Id ) == 0 )
          return static_cast< long >( c );
      }

      return EC_GROUP_NOT_FOUND;
    }

    inline long fGetGroupIndex( const CUnit *_Unit ) const {
      if( _Unit == nullptr )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vGroups.size(); c++ ) {
        if( vGroups [ c ]->fGetUnit( _Unit ) != nullptr )
          return static_cast< long >( c );
      }

      return EC_GROUP_NOT_FOUND;
    }

    inline long fGetGroupIndex( const vector<CUnit *> _Units ) const {
      if( _Units.empty() )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vGroups.size(); c++ ) {
        for( const CUnit *vUnit : _Units ) {
          if( vGroups [ c ]->fGetUnit( vUnit ) != nullptr )
            return static_cast< long >( c );
        }
      }

      return EC_GROUP_NOT_FOUND;
    }

    inline long fGetGroupIndex( const CGroup *_Group ) const {
      if( _Group == nullptr )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vGroups.size(); c++ ) {
        if( vGroups [ c ]->fGetGroup( _Group ) != nullptr )
          return static_cast< long >( c );
      }

      return EC_GROUP_NOT_FOUND;
    }

    inline long fGetGroupIndex( const vector<CGroup *> _Groups ) const {
      if( _Groups.empty() )
        return EC_INVALID_ARGUMENT;

      for( size_t c = 0; c < vGroups.size(); c++ ) {
        for( const CGroup *vGroup : _Groups ) {
          if( vGroups [ c ]->fGetGroup( vGroup ) != nullptr )
            return static_cast< long >( c );
        }
      }

      return EC_GROUP_NOT_FOUND;
    }

    inline CGroup *fChangeGroup( const size_t _Index, CGroup *_Substitute ) {
      if( _Index >= vGroups.size() || _Substitute == nullptr )
        return nullptr;

      CGroup *vGetGroup = vGroups [ _Index ];

      vGroups [ _Index ] = _Substitute;

      return vGetGroup;
    }

    CGroup *fChangeGroup( const wchar_t *_Id, CGroup *_Substitute, const bool _Deep = false );
    CGroup *fChangeGroup( const CUnit *_Unit, CGroup *_Substitute, const bool _Deep = false );
    CGroup *fChangeGroup( const vector<CUnit *> _Units, CGroup *_Substitute, const bool _Deep = false );
    CGroup *fChangeGroup( const CGroup *_Group, CGroup *_Substitute, const bool _Deep = false );
    CGroup *fChangeGroup( const vector<CGroup *> _Groups, CGroup *_Substitute, const bool _Deep = false );

    private:
    wchar_t *vId;

    vector<CGroup *> vGroups;
    vector<CUnit *> vUnits;

    inline void fClearId() {
      if( vId != nullptr ) {
        delete[] vId;
        vId = nullptr;
      }
    }
  };

  class CConfigurator {
    public:
    explicit CConfigurator() noexcept;
    ~CConfigurator();

    inline wchar_t *fGetBuffer() const noexcept { return vBuffer; }

    enErrorCode fLoadBuffer( const wstring _Path );
    enErrorCode fLoadBuffer( const wchar_t *_Buffer );

    enErrorCode fLoadConfiguration( const wstring _Path );
    enErrorCode fLoadConfiguration( const wchar_t *_Buffer, const size_t _StartIndex );

    enErrorCode fSaveConfiguration( const wstring _Path );

    enErrorCode fSaveBuffer( wstring &_Buffer );

    inline vector<CGroup *> fGetGroups() const noexcept {
      return vGroups;
    }

    inline void fSetGroups( vector<CGroup *> _Groups ) noexcept { vGroups = _Groups; }

    inline void fClearGroups() noexcept { vGroups.clear(); }

    inline vector<CUnit *> fGetUnits() const noexcept { return vUnits; }

    inline void fSetUnits( vector<CUnit *> _Units ) noexcept { vUnits = _Units; }

    inline void fClearUnits() noexcept { vUnits.clear(); }

    private:
    const wchar_t *vDefaultExtension = L".cconf";

    inline bool fIsCompendiumFile( const wstring _Path ) const noexcept {
      wstring vGetExtension = path( _Path ).extension();

      if( vGetExtension == vDefaultExtension )
        return true;

      return false;
    }

    wchar_t *vBuffer;

    inline void fClearBuffer() {
      if( vBuffer != nullptr ) {
        delete[] vBuffer;
        vBuffer = nullptr;
      }
    }

    vector<CUnit *> vUnits;
    vector<CGroup *> vGroups;

    inline void fSkipComment( size_t &_Index ) noexcept {
      for( size_t c = _Index + 1; c < wcslen( vBuffer ); c++ ) {
        if( vBuffer [ c ] == L'-' && vBuffer [ c + 1 ] == L'>' ) {
          _Index = c + 2;
          return;
        }
      }
    }

    typedef enum {
      K_GROUP,
      K_UNIT
    } enKeyword;

    long fGetNextKeyword( size_t &_Index ) const noexcept;

    CGroup *fParseGroup( size_t &_Index );
    CUnit *fParseUnit( size_t &_Index );

    wstring fSerializeGroup( const CGroup *_Group, const size_t _Level ) const;
    inline wstring fSerializeUnit( const CUnit *_Unit ) const { return wstring() + L"unit " + _Unit->fGetId() + L":" + _Unit->fGetValue() + L"\n"; }
  };
}
