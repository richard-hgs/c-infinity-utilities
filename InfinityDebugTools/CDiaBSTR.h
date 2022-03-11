#pragma once

#include <assert.h>						// BIBLIOTECA DE ASSERTIONS

class CDiaBSTR {
	private:
		/**
		 * ARMAZENA OS BYTES STREAM DE INFORMACOES
		 */
		BSTR m_bstr;
	public:
		/**
		 * CONSTRUTOR 1 PADRAO
		 */
		CDiaBSTR() {
			m_bstr = NULL;
		}

		/**
		 * DESTRUTOR 1 PADRAO
		 */
		~CDiaBSTR() {
			if (m_bstr != NULL) {
				SysFreeString(m_bstr);
			}
		}

		/**
		 * OPERANDO 1
		 */
		BSTR* operator&() {
			assert(m_bstr == NULL);
			return &m_bstr;
		}

		/**
		 * OPERANDO 2
		 */
		operator BSTR() {
			assert(m_bstr != NULL);
			return m_bstr;
		}
};