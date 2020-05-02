#pragma once
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cmath>

class Utility;

class Word
{
public:
	explicit Word(std::wstring word)
		: m_word(std::move(word)), m_replacedWords(std::unordered_set<std::wstring>())
	{

	}

	Word(const Word& word)
		: m_word(word.m_word), m_replacedWords(word.m_replacedWords)
	{

	}

	Word(Word&& word) noexcept
		: m_word(std::move(word.m_word)), m_replacedWords(std::move(word.m_replacedWords))
	{

	}
	
	Word& operator=(const Word& word)
	{
		if (&word == this) return *this;

		m_word = word.m_word;
		m_replacedWords = word.m_replacedWords;
		return *this;
	}

	Word& operator=(Word&& word) noexcept
	{
		if (&word == this) return *this;

		m_word = std::move(word.m_word);
		m_replacedWords = std::move(word.m_replacedWords);
		return *this;
	}

	friend std::wstringstream& operator<<(std::wstringstream& out, const Word& word);
	friend class Utility;

private:
	Word& replace(const std::wstring& searchValue, const std::wstring& replaceValue,
		bool replaceReplacedWords = false)
	{
		if (!replaceReplacedWords &&
			searchValueContainsReplacedWords(searchValue, replaceValue))
			return *this;

		auto replacing_word = m_word;
		auto regex = std::basic_regex(searchValue, std::regex_constants::ECMAScript |
			std::regex_constants::optimize);

		if (std::regex_search(m_word, regex))
			replacing_word = std::regex_replace(m_word, regex, replaceValue);

		auto begin = std::wsregex_iterator(m_word.cbegin(), m_word.cend(), regex);
		auto end = std::wsregex_iterator();
		auto replaced_words = std::vector<std::wstring>();

		for (auto i = begin; i != end; ++i)
		{
			replaced_words.emplace_back(std::regex_replace(i->str(), regex, replaceValue));
		}

		if (replacing_word != m_word)
		{
			for (const auto& word : replaced_words)
			{
				m_replacedWords.emplace(word);
			}
			m_word = replacing_word;
		}

		return *this;
	}

	Word& replace(const std::wstring& searchValue, const std::function<std::wstring()>& func,
		bool replaceReplacedWords = false)
	{
		auto replace_value = func();

		if (!replaceReplacedWords &&
			searchValueContainsReplacedWords(searchValue, replace_value))
			return *this;

		auto replacing_word = m_word;
		auto regex = std::basic_regex(searchValue, std::regex_constants::ECMAScript |
			std::regex_constants::optimize);

		if (std::regex_search(m_word, regex))
			replacing_word = std::regex_replace(m_word, regex, replace_value);

		auto begin = std::wsregex_iterator(m_word.cbegin(), m_word.cend(), regex);
		auto end = std::wsregex_iterator();
		auto replaced_words = std::vector<std::wstring>();

		for (auto i = begin; i != end; ++i)
		{
			replaced_words.emplace_back(std::regex_replace(i->str(), regex, replace_value));
		}

		if (replacing_word != m_word)
		{
			for (const auto& word : replaced_words)
			{
				m_replacedWords.emplace(word);
			}
			m_word = replacing_word;
		}

		return *this;
	}

	Word& replace(const std::wstring& searchValue,
		const std::function<std::wstring(const std::wstring&, const std::wstring&)> func,
		bool replaceReplacedWords = false)
	{
		auto regex = std::basic_regex(searchValue, std::regex_constants::ECMAScript |
			std::regex_constants::optimize);
		if (!std::regex_search(m_word, regex))
			return *this;

		auto match = std::wsmatch();
		std::regex_match(m_word, match, regex);
		auto replace_value = func(match[1], match[2]);

		if (!replaceReplacedWords &&
			searchValueContainsReplacedWords(searchValue, replace_value))
			return *this;

		auto replacing_word = m_word;

		if (std::regex_search(m_word, regex))
			replacing_word = std::regex_replace(m_word, regex, replace_value);

		auto begin = std::wsregex_iterator(m_word.cbegin(), m_word.cend(), regex);
		auto end = std::wsregex_iterator();
		auto replaced_words = std::vector<std::wstring>();

		for (auto i = begin; i != end; ++i)
		{
			replaced_words.emplace_back(std::regex_replace(i->str(), regex, replace_value));
		}

		if (replacing_word != m_word)
		{
			for (const auto& word : replaced_words)
			{
				m_replacedWords.emplace(word);
			}
			m_word = replacing_word;
		}

		return *this;
	}

private:
	bool searchValueContainsReplacedWords(const std::wstring& searchValue, const std::wstring& replaceValue)
	{
		auto regex = std::basic_regex(searchValue, std::regex_constants::ECMAScript | std::regex_constants::optimize);

		auto result = std::any_of(m_replacedWords.cbegin(), m_replacedWords.cend(), [&](const std::wstring& word) {
			if (std::regex_search(word, regex))
			{
				return std::regex_replace(word, regex, replaceValue) == word;
			}
			return false;
			});

		return result;
	}

	std::wstring m_word;
	std::unordered_set<std::wstring> m_replacedWords;
};

class Owoifier;

class Utility
{
public:
	friend class Owoifier;

	template <typename T = std::wstring>
	static std::deque<T> interleaveArrays(const std::deque<T>& a, const std::deque<T>& b)
	{
		auto arr = std::deque<T>();
		auto observed = a;
		auto other = b;

		while (observed.size() > 0)
		{
			arr.emplace_back(observed[0]);
			observed.pop_front();
			
			std::swap(observed, other);
		}

		if (other.size() > 0)
		{
			std::move(other.begin(), other.end(), std::back_inserter(arr));
		}

		return arr;
	}

private:
	static Word map_O_To_OwO(Word& input)
	{
		auto number = ::roundf(getRandomValue(0.0f, 1.0f));
		return input.replace(L"o", number > 0.0f ? L"owo" : L"o");
	}

	static Word map_Ew_To_UwU(Word& input)
	{
		return input.replace(L"ew", L"uwu");
	}

	static Word map_Hey_To_Hay(Word& input)
	{
		return input.replace(L"([Hh])ey", L"$1ay");
	}

	static Word map_Dead_To_Ded(Word& input)
	{
		return input.replace(L"Dead", L"Ded")
			.replace(L"dead", L"ded");
	}

	static Word map_N_Vowel_T_To_Nd(Word& input)
	{
		return input.replace(L"n[aeiou]*t", L"nd");
	}

	static Word map_Read_To_Wead(Word& input)
	{
		return input.replace(L"Read", L"Wead")
			.replace(L"read", L"wead");
	}

	static Word map_Brackets_To_StarTrails(Word& input)
	{
		return input.replace(L"[({<]", L"｡･:*:･ﾟ★,｡･:*:･ﾟ☆")
			.replace(L"[)}>]", L"☆ﾟ･:*:･｡,★ﾟ･:*:･｡");
	}

	static Word map_PeriodCommaExclamationSemicolon_To_Kaomojis(Word& input)
	{
		auto index = ::floorf(getRandomValue(0.0f, 1.0f) * m_faces.size());
		input = input.replace(L"[.,](?![0-9])", [&]() {
			return L" " + m_faces[index];
			});

		index = ::floorf(getRandomValue(0.0f, 1.0f) * m_faces.size());
		input = input.replace(L"[!;]+", [&]() {
			return L" " + m_faces[index];
			});

		return input;
	}

	static Word map_That_To_Dat(Word& input)
	{
		return input.replace(L"that", L"dat")
			.replace(L"That", L"Dat");
	}

	static Word map_Th_To_F(Word& input)
	{
		return input.replace(L"[Tt]h(?![Ee])", L"f")
			.replace(L"TH(?!E)", L"F");
	}

	static Word map_Le_To_Wal(Word& input)
	{
		return input.replace(L"le$", L"wal");
	}

	static Word map_Ve_To_We(Word& input)
	{
		return input.replace(L"ve", L"we")
			.replace(L"Ve", L"We");
	}

	static Word map_Ry_To_Wwy(Word& input)
	{
		return input.replace(L"ry", L"wwy");
	}

	static Word map_ROrL_To_W(Word& input)
	{
		return input.replace(L"(?:r|l)", L"w")
			.replace(L"(?:R|L)", L"W");
	}

	static Word map_Ll_To_Ww(Word& input)
	{
		return input.replace(L"ll", L"ww");
	}

	static Word map_VowelOrRExceptO_L_To_Wl(Word& input)
	{
		return input.replace(L"[aeiur]l$", L"wl")
			.replace(L"[AEIUR]([lL])$", L"W$1");
	}

	static Word map_Old_To_Owld(Word& input)
	{
		return input.replace(L"([Oo])ld", L"$1wld")
			.replace(L"OLD", L"OWLD");
	}

	static Word map_Ol_To_Owl(Word& input)
	{
		return input.replace(L"([Oo])l", L"$1wl")
			.replace(L"OL", L"OWL");
	}

	static Word map_LOrR_O_To_Wo(Word& input)
	{
		return input.replace(L"[lr]o", L"wo")
			.replace(L"[LR]([oO])", L"W$1");
	}

	static Word map_SpecificConsonants_O_To_Letter_And_Wo(Word& input)
	{
		return input.replace(L"([bcdfghjkmnpqstxyz])o", L"$1wo")
			.replace(L"([BCDFGHJKMNPQSTXYZ])([oO])", [&](const std::wstring& m1,
				const std::wstring& m2) {

					std::wstring upper = L"";
					for (const auto& c : m2)
					{
						upper += c;
					}

					return m1 + (upper == m2 ? L"W" : L"w") + m2;
				});
	}

	static Word map_VOrW_Le_To_Wal(Word& input)
	{
		return input.replace(L"[vw]le", L"wal");
	}

	static Word map_Fi_To_Fwi(Word& input)
	{
		return input.replace(L"([Ff])i", L"$1wi")
			.replace(L"FI", L"FWI");
	}

	static Word map_Ver_To_Wer(Word& input)
	{
		return input.replace(L"([Vv])er", L"wer");
	}

	static Word map_Poi_To_Pwoi(Word& input)
	{
		return input.replace(L"([Pp])oi", L"$1woi");
	}

	static Word map_SpecificConsonants_Le_To_Letter_And_Wal(Word& input)
	{
		return input.replace(L"([DdFfGgHhJjPpQqRrSsTtXxYyZz])le$", L"$1wal");
	}

	static Word map_Consonant_R_To_Consonant_W(Word& input)
	{
		return input.replace(L"([BbCcDdFfGgKkPpQqSsTtWwXxZz])r", L"$1w");
	}

	static Word map_Ly_To_Wy(Word& input)
	{
		return input.replace(L"ly", L"wy")
			.replace(L"Ly", L"Wy");
	}

	static Word map_Ple_To_Pwe(Word& input)
	{
		return input.replace(L"([Pp])le", L"1we");
	}

	static Word map_Nr_To_Nw(Word& input)
	{
		return input.replace(L"nr", L"nw")
			.replace(L"NR", L"NW");
	}

	static Word map_Fuc_To_Fwuc(Word& input)
	{
		return input.replace(L"([Ff])uc", L"$1wuc");
	}

	static Word map_Mom_To_Mwom(Word& input)
	{
		return input.replace(L"([Mm])om", L"$1wom");
	}

	static Word map_Me_To_Mwe(Word& input)
	{
		return input.replace(L"([Mm])e", L"$1we");
	}

	static Word map_NVowel_To_Ny(Word& input)
	{
		return input.replace(L"n([aeiou])", L"ny$1")
			.replace(L"N([aeiou])", L"Ny$1")
			.replace(L"N([AEIOU])", L"NY$1");
	}

	static Word map_Ove_To_Uv(Word& input)
	{
		return input.replace(L"ove", L"uv")
			.replace(L"OVE", L"UV");
	}

	static Word map_Haha_To_HehexD(Word& input)
	{
		return input.replace(L"\\b(ha|hah|heh|hehe)+\\b", L"hehe xD");
	}

	static Word map_The_To_Teh(Word& input)
	{
		return input.replace(L"\\b([Tt])he\\b", L"$1eh");
	}

	static Word map_You_To_U(Word& input)
	{
		return input.replace(L"\\bYou\\b", L"U")
			.replace(L"\\byou\\b", L"u");
	}

	static Word map_Time_To_Tim(Word& input)
	{
		return input.replace(L"\\b([Tt])ime\\b", L"$1im");
	}

	static Word map_Over_To_Owor(Word& input)
	{
		return input.replace(L"([Oo])ver", L"$1wor");
	}

	static Word map_Worse_To_Wose(Word& input)
	{
		return input.replace(L"([Ww])orse", L"$1ose");
	}

	template <typename T = std::mt19937, size_t N = T::state_size>
	static auto getRandomSeededEngine() -> typename std::enable_if<N != 0, T>::type
	{
		typename T::result_type data[N];
		std::random_device rd;
		std::generate(std::begin(data), std::end(data), std::ref(rd));
		std::seed_seq seeds(std::begin(data), std::end(data));
		T engine(seeds);
		return engine;
	}

	template <typename T = int>
	static T getRandomValue(T upper, T lower)
	{
		if constexpr (std::is_floating_point<T>::value)
		{
			std::uniform_real_distribution<T> rng(upper, lower);
			return rng(m_rng);
		}
		else if constexpr (std::is_integral<T>::value)
		{
			std::uniform_int_distribution<T> rng(upper, lower);
			return rng(m_rng);
		}
		else
		{
			throw std::runtime_error("The type T is not available for generating random numbers.");
		}
	}

	inline static const std::vector<std::wstring> m_faces = {
		L"(・`ω´・)", L";;w;;", L"owo", L"UwU", L">w<", L"^w^", L"(* ^ ω ^)",
		L"(⌒ω⌒)", L"ヽ(*・ω・)ﾉ", L"(o´∀`o)", L"(o･ω･o)", L"＼(＾▽＾)／"
	};

	inline static std::mt19937 m_rng = getRandomSeededEngine();

public:
	inline static const std::vector<Word(*)(Word&)> m_specificWordMappingList = {
		map_Fuc_To_Fwuc, map_Mom_To_Mwom, map_Time_To_Tim, map_Me_To_Mwe,
		map_NVowel_To_Ny, map_Over_To_Owor, map_Ove_To_Uv, map_Haha_To_HehexD,
		map_The_To_Teh, map_You_To_U, map_Read_To_Wead, map_Worse_To_Wose
	};

	inline static const std::vector<Word(*)(Word&)> m_uvuMappingList = {
		map_O_To_OwO, map_Ew_To_UwU, map_Hey_To_Hay, map_Dead_To_Ded,
		map_N_Vowel_T_To_Nd
	};

	inline static const std::vector<Word(*)(Word&)> m_uwuMappingList = {
		map_Brackets_To_StarTrails, map_PeriodCommaExclamationSemicolon_To_Kaomojis,
		map_That_To_Dat, map_Th_To_F, map_Le_To_Wal, map_Ve_To_We, map_Ry_To_Wwy,
		map_ROrL_To_W
	};

	inline static const std::vector<Word(*)(Word&)> m_owoMappingList = {
		map_Ll_To_Ww, map_VowelOrRExceptO_L_To_Wl, map_Old_To_Owld,
		map_Ol_To_Owl, map_LOrR_O_To_Wo, map_SpecificConsonants_O_To_Letter_And_Wo,
		map_VOrW_Le_To_Wal, map_Fi_To_Fwi, map_Ver_To_Wer, map_Poi_To_Pwoi,
		map_SpecificConsonants_Le_To_Letter_And_Wal,
		map_Consonant_R_To_Consonant_W,
		map_Ly_To_Wy, map_Ple_To_Pwe, map_Nr_To_Nw
	};
};

class Owoifier
{
public:
	enum class OwoifyLevel
	{
		Owo, Uwu, Uvu
	};

	static std::wstring owoify(const std::string& value, OwoifyLevel level = OwoifyLevel::Owo)
	{
		auto word_regex = std::basic_regex(L"[^\\s]+", std::regex_constants::ECMAScript | std::regex_constants::optimize);
		
		auto w_value = stringToWstring(value);
		
		auto word_begin = std::wsregex_iterator(w_value.cbegin(), w_value.cend(), word_regex);
		auto word_end = std::wsregex_iterator();

		auto space_regex = std::basic_regex(L"\\s+", std::regex_constants::ECMAScript | std::regex_constants::optimize);

		auto space_begin = std::wsregex_iterator(w_value.cbegin(), w_value.cend(), space_regex);
		auto space_end = std::wsregex_iterator();

		auto words = std::deque<Word>();
		std::transform(word_begin, word_end, std::back_inserter(words),
			[&](const std::wsmatch& match) {
				return Word(match.str());
			});

		auto spaces = std::deque<Word>();
		std::transform(space_begin, space_end, std::back_inserter(spaces),
			[&](const std::wsmatch& match) {
				return Word(match.str());
			});

		auto transformed_words = std::deque<Word>();
		std::transform(words.begin(), words.end(), std::back_inserter(transformed_words),
			[&](Word& word) {

				for (const auto& func : Utility::m_specificWordMappingList)
				{
					word = func(word);
				}

				switch (level)
				{
				case Owoifier::OwoifyLevel::Uvu:
					for (const auto& func : Utility::m_uvuMappingList)
					{
						word = func(word);
					}
				case Owoifier::OwoifyLevel::Uwu:
					for (const auto& func : Utility::m_uwuMappingList)
					{
						word = func(word);
					}
				case Owoifier::OwoifyLevel::Owo:
					for (const auto& func : Utility::m_owoMappingList)
					{
						word = func(word);
					}
					break;
				default:
					break;
				}
				
				return word;
			});

		auto result = Utility::interleaveArrays(words, spaces);
		return join(result);
	}

private:
	static std::wstring stringToWstring(const std::string& source)
	{
		auto str = std::wstring();
		str.reserve(source.size());

		for (const auto& c : source)
		{
			str += c;
		}

		return str;
	}

	static std::wstring join(const std::deque<Word>& words)
	{
		auto ss = std::wstringstream();

		for (const auto& word : words)
		{
			ss << word;
		}

		return ss.str();
	}
};

inline std::wstringstream& operator<<(std::wstringstream& out, const Word& word)
{
	out << word.m_word;
	return out;
}