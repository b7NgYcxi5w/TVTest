#ifndef TVTEST_UTIL_H
#define TVTEST_UTIL_H


#include "HelperClass/StdUtil.h"
#include "StringUtility.h"
#include "PathUtil.h"


namespace TVTest
{

	inline bool operator==(const RECT &rc1, const RECT &rc2) {
		return rc1.left == rc2.left && rc1.top == rc2.top && rc1.right == rc2.right && rc1.bottom == rc2.bottom;
	}
	inline bool operator!=(const RECT &rc1, const RECT &rc2) { return !(rc1 == rc2); }
	inline bool operator==(const POINT &pt1, const POINT &pt2) { return pt1.x == pt2.x && pt1.y == pt2.y; }
	inline bool operator!=(const POINT &pt1, const POINT &pt2) { return !(pt1 == pt2); }

	int HexCharToInt(TCHAR Code);
	unsigned int HexStringToUInt(LPCTSTR pszString, int Length, LPCTSTR *ppszEnd = nullptr);

	bool IsRectIntersect(const RECT *pRect1, const RECT *pRect2);

	float LevelToDeciBel(int Level);

	COLORREF MixColor(COLORREF Color1, COLORREF Color2, BYTE Ratio = 128);
	COLORREF HSVToRGB(double Hue, double Saturation, double Value);
	void RGBToHSV(
		BYTE Red, BYTE Green, BYTE Blue,
		double *pHue, double *pSaturation, double *pValue);

	inline DWORD TickTimeSpan(DWORD Start, DWORD End) { return End - Start; }
	inline ULONGLONG TickTimeSpan(ULONGLONG Start, ULONGLONG End) { _ASSERT(Start <= End); return End - Start; }

	extern __declspec(selectany) const FILETIME FILETIME_NULL = {0, 0};
	constexpr LONGLONG FILETIME_MILLISECOND = 10000LL;
	constexpr LONGLONG FILETIME_SECOND      = 1000LL * FILETIME_MILLISECOND;
	constexpr LONGLONG FILETIME_MINUTE      = 60LL * FILETIME_SECOND;
	constexpr LONGLONG FILETIME_HOUR        = 60LL*FILETIME_MINUTE;
	FILETIME &operator+=(FILETIME &ft, LONGLONG Offset);
	LONGLONG operator-(const FILETIME &ft1, const FILETIME &ft2);
	namespace TimeConsts {
		constexpr LONGLONG SYSTEMTIME_SECOND = 1000LL;
		constexpr LONGLONG SYSTEMTIME_MINUTE = 60LL * SYSTEMTIME_SECOND;
		constexpr LONGLONG SYSTEMTIME_HOUR   = 60LL * SYSTEMTIME_MINUTE;
		constexpr LONGLONG SYSTEMTIME_DAY    = 24LL * SYSTEMTIME_HOUR;
	}
	int CompareSystemTime(const SYSTEMTIME *pTime1, const SYSTEMTIME *pTime2);
	bool OffsetSystemTime(SYSTEMTIME *pTime, LONGLONG Offset);
	LONGLONG DiffSystemTime(const SYSTEMTIME *pStartTime, const SYSTEMTIME *pEndTime);
	void GetLocalTimeAsFileTime(FILETIME *pTime);
	void SystemTimeTruncateDay(SYSTEMTIME *pTime);
	void SystemTimeTruncateHour(SYSTEMTIME *pTime);
	void SystemTimeTruncateMinute(SYSTEMTIME *pTime);
	void SystemTimeTruncateSecond(SYSTEMTIME *pTime);
	bool GetJSTTimeZoneInformation(TIME_ZONE_INFORMATION *pInfo);
	int CalcDayOfWeek(int Year, int Month, int Day);
	LPCTSTR GetDayOfWeekText(int DayOfWeek);

	bool CopyTextToClipboard(HWND hwndOwner, LPCTSTR pszText);

	void ClearMenu(HMENU hmenu);
	int CopyToMenuText(LPCTSTR pszSrcText, LPTSTR pszDstText, int MaxLength);
	String FormatMenuString(const String &Str);
	String FormatMenuString(LPCWSTR pszText);

	void InitOpenFileName(OPENFILENAME *pofn);
	bool FileOpenDialog(OPENFILENAME *pofn);
	bool FileSaveDialog(OPENFILENAME *pofn);

	void ForegroundWindow(HWND hwnd);

	bool ChooseColorDialog(HWND hwndOwner, COLORREF *pcrResult);
	bool ChooseFontDialog(HWND hwndOwner, LOGFONT *plf, int *pPointSize = nullptr);
	bool BrowseFolderDialog(HWND hwndOwner, LPTSTR pszDirectory, LPCTSTR pszTitle);

	bool CompareLogFont(const LOGFONT *pFont1, const LOGFONT *pFont2);
	int PixelsToPoints(int Pixels);
	int PointsToPixels(int Points);
	int CalcFontPointHeight(HDC hdc, const LOGFONT *pFont);

	int GetErrorText(DWORD ErrorCode, LPTSTR pszText, int MaxLength);

	bool IsEqualFileName(LPCWSTR pszFileName1, LPCWSTR pszFileName2);
	enum class FileNameValidateFlag : unsigned int {
		None           = 0x0000U,
		Wildcard       = 0x0001U,
		AllowDelimiter = 0x0002U,
	};
	TVTEST_ENUM_FLAGS(FileNameValidateFlag)
	bool IsValidFileName(LPCTSTR pszFileName, FileNameValidateFlag Flags = FileNameValidateFlag::None, String *pMessage = nullptr);
	bool MakeUniqueFileName(
		String *pFileName, size_t MaxLength = MAX_PATH - 1,
		LPCTSTR pszNumberFormat = nullptr);
	bool GetAbsolutePath(LPCTSTR pszFilePath, LPTSTR pszAbsolutePath, int MaxLength);
	bool GetAbsolutePath(const String &FilePath, String *pAbsolutePath);

	HICON CreateIconFromBitmap(HBITMAP hbm, int IconWidth, int IconHeight, int ImageWidth = 0, int ImageHeight = 0);
	bool SaveIconFromBitmap(
		LPCTSTR pszFileName, HBITMAP hbm,
		int IconWidth, int IconHeight, int ImageWidth = 0, int ImageHeight = 0);
	HICON CreateEmptyIcon(int Width, int Height, int BitsPerPixel = 1);
	enum class IconSizeType {
		Small,
		Normal,
	};
	bool GetStandardIconSize(IconSizeType Size, int *pWidth, int *pHeight);
	HICON LoadIconStandardSize(HINSTANCE hinst, LPCTSTR pszName, IconSizeType Size);
	HICON LoadSystemIcon(LPCTSTR pszName, IconSizeType Size);
	HICON LoadSystemIcon(LPCTSTR pszName, int Width, int Height);

	class CStaticStringFormatter
	{
	public:
		CStaticStringFormatter(LPTSTR pBuffer, size_t BufferLength);
		size_t Length() const { return m_Length; }
		bool IsEmpty() const { return m_Length == 0; }
		LPCTSTR GetString() const { return m_pBuffer; }
		void Clear();
		void Append(LPCTSTR pszString);
		void AppendFormat(LPCTSTR pszFormat, ...);
		void AppendFormatV(LPCTSTR pszFormat, va_list Args);
		void RemoveTrailingWhitespace();

	private:
		const LPTSTR m_pBuffer;
		const size_t m_BufferLength;
		size_t m_Length;
	};

	class CLocalTime
	{
	protected:
		FILETIME m_Time;

	public:
		CLocalTime();
		CLocalTime(const FILETIME &Time);
		CLocalTime(const SYSTEMTIME &Time);
		virtual ~CLocalTime();
		bool operator==(const CLocalTime &Time) const;
		bool operator!=(const CLocalTime &Time) const { return !(*this == Time); }
		bool operator<(const CLocalTime &Time) const;
		bool operator>(const CLocalTime &Time) const;
		bool operator<=(const CLocalTime &Time) const;
		bool operator>=(const CLocalTime &Time) const;
		CLocalTime &operator+=(LONGLONG Offset);
		CLocalTime &operator-=(LONGLONG Offset) { return *this += -Offset; }
		LONGLONG operator-(const CLocalTime &Time) const;
		void SetCurrentTime();
		bool GetTime(FILETIME *pTime) const;
		bool GetTime(SYSTEMTIME *pTime) const;
	};

	class CGlobalLock
	{
		HANDLE m_hMutex;
		bool m_fOwner;

		// delete
		CGlobalLock(const CGlobalLock &);
		CGlobalLock &operator=(const CGlobalLock &);

	public:
		CGlobalLock();
		~CGlobalLock();
		bool Create(LPCTSTR pszName, bool fInheritHandle = false);
		bool Open(
			LPCTSTR pszName,
			DWORD DesiredAccess = MUTEX_ALL_ACCESS,
			bool fInheritHandle = false);
		bool Wait(DWORD Timeout = INFINITE);
		void Close();
		void Release();
	};

	class CBasicSecurityAttributes
		: public SECURITY_ATTRIBUTES
	{
	public:
		CBasicSecurityAttributes();

		CBasicSecurityAttributes(const CBasicSecurityAttributes &) = delete;
		CBasicSecurityAttributes &operator=(const CBasicSecurityAttributes &) = delete;

		bool Initialize();

	private:
		SECURITY_DESCRIPTOR m_SecurityDescriptor;
	};

	namespace Util
	{

		template<typename T> T *GetLibraryFunction(HMODULE hLib, LPCSTR pszFunc)
		{
			return reinterpret_cast<T*>(::GetProcAddress(hLib, pszFunc));
		}

		template<typename T> T *GetModuleFunction(LPCTSTR pszModule, LPCSTR pszFunc)
		{
			return reinterpret_cast<T*>(::GetProcAddress(::GetModuleHandle(pszModule), pszFunc));
		}

#define GET_LIBRARY_FUNCTION(hLib,Func) \
	Util::GetLibraryFunction<decltype(Func)>(hLib,#Func)
#define GET_MODULE_FUNCTION(pszModule,Func) \
	Util::GetModuleFunction<decltype(Func)>(pszModule,#Func)

		HMODULE LoadSystemLibrary(LPCTSTR pszName);

		typedef ULONGLONG TickCountType;
		inline TickCountType GetTickCount() { return ::GetTickCount64(); }

		namespace OS
		{

			bool IsWindowsXP();
			bool IsWindowsVista();
			bool IsWindows7();
			bool IsWindows8();
			bool IsWindows8_1();
			bool IsWindows10();
			bool IsWindows10AnniversaryUpdate();
			bool IsWindowsXPOrLater();
			bool IsWindowsVistaOrLater();
			bool IsWindows7OrLater();
			bool IsWindows8OrLater();
			bool IsWindows8_1OrLater();
			bool IsWindows10OrLater();
			bool IsWindows10AnniversaryUpdateOrLater();

		}	// namespace OS

		class CRect
			: public ::RECT
		{
		public:
			CRect() { Empty(); }

			CRect(int Left, int Top, int Right, int Bottom)
			{
				Set(Left, Top, Right, Bottom);
			}

			CRect &operator=(const RECT &Op)
			{
				left = Op.left;
				top = Op.top;
				right = Op.right;
				bottom = Op.bottom;
				return *this;
			}

			bool operator==(const CRect &Op)
			{
				return left == Op.left
					&& top == Op.top
					&& right == Op.right
					&& bottom == Op.bottom;
			}

			bool operator!=(const CRect &Op) { return !(*this == Op); }

			void Set(int Left, int Top, int Right, int Bottom)
			{
				left = Left; top = Top; right = Right; bottom = Bottom;
			}

			void Empty() { left = 0; top = 0; right = 0; bottom = 0; }

			bool IsEmpty() const { return left == right && top == bottom; }

			int GetWidth() const { return right - left; }

			int GetHeight() const { return bottom - top; }

			bool Intersect(const RECT &Rect)
			{
				RECT rc;
				bool fResult =::IntersectRect(&rc, this, &Rect) != FALSE;
				*this = rc;
				return fResult;
			}

			bool Union(const RECT &Rect)
			{
				RECT rc;
				bool fResult =::UnionRect(&rc, this, &Rect) != FALSE;
				*this = rc;
				return fResult;
			}
		};

		class CClock
		{
		public:
			void Start() { m_Clock = ::GetTickCount(); }
			DWORD GetSpan() const { return TickTimeSpan(m_Clock, ::GetTickCount()); }

		private:
			DWORD m_Clock;
		};

		class CWaitCursor
		{
		public:
			CWaitCursor()
				: m_hcurOld(::SetCursor(::LoadCursor(nullptr, IDC_WAIT)))
			{
			}

			~CWaitCursor()
			{
				::SetCursor(m_hcurOld);
			}

		private:
			HCURSOR m_hcurOld;
		};

		class CTimer
		{
		public:
			CTimer();
			virtual ~CTimer();
			bool Begin(DWORD DueTime, DWORD Period);
			void End();

		protected:
			virtual void OnTimer() = 0;

		private:
			static void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

			HANDLE m_hTimer;
		};

		template<typename T, std::size_t N> class CTempBuffer
		{
		public:
			CTempBuffer() : m_pBuffer(m_FixedBuffer) {}
			CTempBuffer(std::size_t Elements) { Allocate_(Elements); }
			~CTempBuffer() { Free(); }
			T &operator[](std::size_t i) { return m_pBuffer[i]; }
			const T &operator[](std::size_t i) const { return m_pBuffer[i]; }
			void Allocate(std::size_t Elements)
			{
				Free();
				Allocate_(Elements);
			}
			void Free()
			{
				if (m_pBuffer != m_FixedBuffer) {
					delete [] m_pBuffer;
					m_pBuffer = m_FixedBuffer;
				}
			}
			T *GetBuffer() { return m_pBuffer; }
			const T *GetBuffer() const { return m_pBuffer; }

		private:
			void Allocate_(std::size_t Elements)
			{
				if (Elements <= N) {
					m_pBuffer = m_FixedBuffer;
				} else {
					m_pBuffer = new T[Elements];
				}
			}

			T *m_pBuffer;
			T m_FixedBuffer[N];
		};

	}	// namespace Util

}	// namespace TVTest


#include "LibISDB/LibISDB/Utilities/Lock.hpp"

namespace TVTest
{

	using LibISDB::MutexLock;
	using LibISDB::BlockLock;
	using LibISDB::TryBlockLock;

}	// namespace TVTest


#endif
