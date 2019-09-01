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

#include "Compendium.hpp"

using Compendium::CUnit;

CUnit::CUnit() noexcept:
  vId( nullptr ), vValue( nullptr ) {}

CUnit::CUnit( const CUnit &_Copy ) noexcept:
  vId( _Copy.fGetId() ), vValue( _Copy.fGetValue() ) {}

CUnit::CUnit( const wchar_t *_Id, const wchar_t *_Value ) noexcept:
  vId( nullptr ), vValue( nullptr ) {
  if( _Id != nullptr ) {
    size_t vSourceSize = wcslen( _Id ) + 1;
    vId = new wchar_t [ wcslen( _Id ) + 1 ];
    wcscpy_s( vId, vSourceSize, _Id );
  }

  if( _Value != nullptr ) {
    size_t vSourceSize = wcslen( _Value ) + 1;
    vValue = new wchar_t [ wcslen( _Value ) + 1 ];
    wcscpy_s( vValue, vSourceSize, _Value );
  }
}

CUnit::~CUnit() {
  fClearId();
  fClearValue();
}
