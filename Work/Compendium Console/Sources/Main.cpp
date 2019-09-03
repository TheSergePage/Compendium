/*
© 2019, Dark Orb.

The license version - 1.0

This license is hereby grants to any person who obtained a copy of this software the next rights to:
1. Use and do reverse-engineering of compiled version of this software at no cost, without any restrictions, in non-commercial and commercial purposes
2. Use source codes of this software at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
3. Copy and distribute without any fee
4. Create a copy of the original repository and / or create own derivative software for non-commercial,  academic and / or scientific purposes only

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

#include <Compendium.hpp>

using namespace Compendium;

using std::wstring_convert;

#include <iostream>

using std::wcout;
using std::endl;

using std::move;

wstring_convert<codecvt_utf8_utf16<wchar_t>> vConverter;

void fInsertAlign( const size_t _Level ) {
  for( size_t c = 0; c < _Level; c++ )
    wcout << L" ";
}

void fShowUnit( const CUnit *_Unit, const size_t _Level ) {
  fInsertAlign( _Level );
  wcout << L"Unit id - " << _Unit->fGetId() << endl;

  fInsertAlign( _Level );
  wcout << L"Unit value - " << _Unit->fGetValue() << endl << endl;
}

void fShowGroup( const CGroup *_Group, const size_t _Level ) {
  fInsertAlign( _Level );
  wcout << L"Group id - " << _Group->fGetId() << endl;

  fInsertAlign( _Level );
  wcout << L"-----------" << endl;

  fInsertAlign( _Level );
  wcout << L"Inner units:";

  if( _Group->fGetUnits().size() == 0 )
    wcout << L" Have no units" << endl;
  else {
    wcout << endl;

    for( const CUnit *vUnit : _Group->fGetUnits() )
      fShowUnit( vUnit, _Level + 1 );
  }

  fInsertAlign( _Level );
  wcout << L"-----------" << endl;

  fInsertAlign( _Level );
  wcout << L"Inner groups:";

  if( _Group->fGetGroups().size() == 0 )
    wcout << L" Have no groups" << endl;
  else {
    wcout << endl;

    for( const CGroup *vGroup : _Group->fGetGroups() )
      fShowGroup( vGroup, _Level + 1 );
  }
}

void fShowHowToUse() {
  wcout << L"How to use:" << endl;
  wcout << L"-cmd - command[View/Create/Change/Encode/Decode]" << endl << endl;

  wcout << L"-lpath - path to the file for load" << endl;
  wcout << L"-spath - path to the file or folder for save" << endl << endl;

  wcout << L"-tg - target group for changing" << endl;
  wcout << L"-tu - target unit for changing" << endl << endl;
}

void fShowConfiguration( const CConfigurator _Configurator ) {
  wcout << L"Units:";

  if( _Configurator.fGetUnits().size() == 0 )
    wcout << L"Have no units" << endl;
  else {
    wcout << endl;

    for( const CUnit *vUnit : _Configurator.fGetUnits() )
      fShowUnit( vUnit, 0 );
  }

  wcout << L"*************************" << endl;

  wcout << L"Groups:";

  if( _Configurator.fGetGroups().size() == 0 )
    wcout << L"Have no groups" << endl;
  else {
    wcout << endl;

    for( const CGroup *vGroup : _Configurator.fGetGroups() )
      fShowGroup( vGroup, 0 );
  }

  wcout << L"=========================" << endl;
}

int __cdecl main( int argc, char **argv ) {
  wcout << L">>>Compendium Console 2.1" << endl;
  wcout << L">>>Compendium Lib 2.1" << endl << endl;

  wcout << L"=========================" << endl << endl;

  if( argc == 1 ) {
    fShowHowToUse();
    return 0;
  }

  size_t vCommand = 0;

  wstring vLoadPath, vSavePath;
  wstring vTargetGroup, vTargetUnit;

  for( size_t c = 1; c < static_cast< size_t >( argc ); c++ ) {
    if( strcmp( argv [ c ], "-cmd" ) == 0 ) {
      if( strcmp( argv [ c + 1 ], "View" ) == 0 ) {
        ++c;
        vCommand = 0;
      }
      else if( strcmp( argv [ c + 1 ], "Create" ) == 0 ) {
        ++c;
        vCommand = 1;
      }
      else if( strcmp( argv [ c + 1 ], "Change" ) == 0 ) {
        ++c;
        vCommand = 2;
      }
      else {
        wcout << L">>>Invalid argument value. Argument - cmd : " << argv [ c + 1 ] << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }
    }
    else if( strcmp( argv [ c ], "-lpath" ) == 0 ) {
      vLoadPath = vConverter.from_bytes( argv [ ++c ] );
    }
    else if( strcmp( argv [ c ], "-spath" ) == 0 ) {
      vSavePath = vConverter.from_bytes( argv [ ++c ] );
    }
    else if( strcmp( argv [ c ], "-tg" ) == 0 ) {
      vTargetGroup = vConverter.from_bytes( argv [ ++c ] );
    }
    else if( strcmp( argv [ c ], "-tu" ) == 0 ) {
      vTargetUnit = vConverter.from_bytes( argv [ ++c ] );
    }
    else {
      wcout << L">>>Unknown argument. Argument - " << argv [ c ] << endl;

      wcout << L"=========================" << endl;

      fShowHowToUse();

      return -1;
    }
  }

  CConfigurator vConfigurator;

  vector<CGroup *> vGroups;
  switch( vCommand ) {
    //View
    case 0:
    {
      if( vLoadPath.empty() ) {
        wcout << L">>>Load path is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      wcout << L"Load result - " << vConfigurator.fLoadConfiguration( vLoadPath ) << endl << endl;

      fShowConfiguration( vConfigurator );
    }
    break;

    //Create
    case 1:
    {
      if( vSavePath.empty() ) {
        wcout << L">>>Save path is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      CGroup *vGroup = new CGroup( wstring( L"AboutMe" ).c_str() );

      vGroup->fAddUnit( new CUnit( wstring( L"myName" ).c_str(), wstring( L"Serge Page" ).c_str() ) );
      vGroup->fAddUnit( new CUnit( wstring( L"profession" ).c_str(), wstring( L"Programmer" ).c_str() ) );
      vGroup->fAddUnit( new CUnit( wstring( L"age" ).c_str(), wstring( L"20" ).c_str() ) );

      vGroups.push_back( vGroup );

      vConfigurator.fSetGroups( vGroups );

      wcout << L"Save result - " << vConfigurator.fSaveConfiguration( vSavePath ) << endl;
    }
    break;

    //Change
    case 2:
    {
      if( vLoadPath.empty() ) {
        wcout << L">>>Load path is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      if( vSavePath.empty() ) {
        wcout << L">>>Save path is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      wcout << L"Load result - " << vConfigurator.fLoadConfiguration( vLoadPath ) << endl;

      if( vTargetGroup.empty() ) {
        wcout << L">>>Target group id is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      if( vTargetUnit.empty() ) {
        wcout << L">>>Target unit id is empty" << endl;

        wcout << L"=========================" << endl;

        fShowHowToUse();

        return -1;
      }

      for( CGroup *&vGroup : vConfigurator.fGetGroups() ) {
        if( vGroup->fGetId() == vTargetGroup ) {
          vGroup->fAddGroup( new CGroup( wstring( L"aNewGroup" ).c_str() ) );

          for( CUnit *&vUnit : vGroup->fGetUnits() ) {
            if( vUnit->fGetId() == vTargetUnit ) {
              vUnit->fSetValue( wstring( L"It's a new value" ).c_str() );

              wcout << L"Save result - " << vConfigurator.fSaveConfiguration( vSavePath ) << endl;

              return 0;
            }
          }

          wcout << L">>>The required unit not found" << endl;

          wcout << L"=========================" << endl;

          fShowHowToUse();

          return -1;
        }
      }

      wcout << L">>>The required group not found" << endl;

      wcout << L"=========================" << endl;

      fShowHowToUse();

      return -1;
    }
    break;
  }

  return 0;
}
