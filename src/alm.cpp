/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <alm.hpp>

/**
 * event Метод проверки на спец-слово
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Alm::event(const size_t idw) const noexcept {
	// Если это не спец-слова, формируем слово
	return (
		(idw != size_t(token_t::num)) &&
		(idw != size_t(token_t::unk)) &&
		(idw != size_t(token_t::url)) &&
		(idw != size_t(token_t::abbr)) &&
		(idw != size_t(token_t::date)) &&
		(idw != size_t(token_t::time)) &&
		(idw != size_t(token_t::anum)) &&
		(idw != size_t(token_t::math)) &&
		(idw != size_t(token_t::specl)) &&
		(idw != size_t(token_t::aprox)) &&
		(idw != size_t(token_t::start)) &&
		(idw != size_t(token_t::range)) &&
		(idw != size_t(token_t::score)) &&
		(idw != size_t(token_t::punct)) &&
		(idw != size_t(token_t::dimen)) &&
		(idw != size_t(token_t::fract)) &&
		(idw != size_t(token_t::isolat)) &&
		(idw != size_t(token_t::finish))
	);
}
/**
 * findWord Метод поиска слова в словаре
 * @param idw идентификатор слова
 * @return    найденное слово в словаре
 */
const anyks::word_t * anyks::Alm::findWord(const size_t idw) const noexcept {
	// Результат работы функции
	const word_t * result = nullptr;
	// Если идентификатор передан
	if(idw > 0){
		// Если функция внешнего словаря установлена, используем её
		if(this->getWord != nullptr) result = this->getWord(idw);
		// Если же функция не указана используем внутренний словарь
		else if(!this->vocab.empty()) {
			// Получаем слово по его идентификатору
			auto it = this->vocab.find(idw);
			// Если слово получено
			if(it != this->vocab.end()) result = &it->second;
		}
	}
	// Выводим результат
	return result;
}
/**
 * word Метод извлечения слова по его идентификатору
 * @param idw идентификатор слова
 * @param ups регистры слова
 * @return    слово соответствующее идентификатору
 */
const anyks::word_t anyks::Alm::word(const size_t idw, const size_t ups) const noexcept {
	// Результат работы функции
	word_t result = L"";
	// Определяем тип записи
	switch(idw){
		// Если это токен арабского или римского числа
		case u_short(token_t::num): result = L"<num>"; break;
		// Если это токен неизвестного слова
		case u_short(token_t::unk): result = L"<unk>"; break;
		// Если это токен url адреса
		case u_short(token_t::url): result = L"<url>"; break;
		// Если это токен начала предложения
		case u_short(token_t::start): result = L"<s>"; break;
		// Если это токен аббревиатуры
		case u_short(token_t::abbr): result = L"<abbr>"; break;
		// Если это токен даты
		case u_short(token_t::date): result = L"<date>"; break;
		// Если это токен времени
		case u_short(token_t::time): result = L"<time>"; break;
		// Если это токен псевдо-числа
		case u_short(token_t::anum): result = L"<anum>"; break;
		// Если это символ математической операции
		case u_short(token_t::math): result = L"<math>"; break;
		// Если это токен конца предложения
		case u_short(token_t::finish): result = L"</s>"; break;
		// Если это спец-символ
		case u_short(token_t::specl): result = L"<specl>"; break;
		// Если это токен диапазона чисел
		case u_short(token_t::range): result = L"<range>"; break;
		// Если это токен знака пунктуации
		case u_short(token_t::punct): result = L"<punct>"; break;
		// Если это токен приблизительного числа
		case u_short(token_t::aprox): result = L"<aprox>"; break;
		// Если это токен числового счёта
		case u_short(token_t::score): result = L"<score>"; break;
		// Если это токен габаритных размеров
		case u_short(token_t::dimen): result = L"<dimen>"; break;
		// Если это токен числовой дроби
		case u_short(token_t::fract): result = L"<fract>"; break;
		// Если это токен изоляции
		case u_short(token_t::isolat): result = L"<isolat>"; break;
		// Если это нормальное слово
		default: {
			// Получаем слово по его идентификатору
			const word_t * word = this->findWord(idw);
			// Если слово получено
			if(word != nullptr){
				// Получаем данные слова
				word_t tmp = (* word);
				// Устанавливаем регистры слова
				tmp.setUppers(ups);
				// Выводим результат
				result = tmp;
			// Устанавливаем как неизвестное слово
			} else result = L"<unk>";
		}
	}
	// Выводим результат
	return result;
}
/**
 * set Метод установки последовательности в словарь
 * @param seq последовательность слов для установки
 */
void anyks::Alm::set(const vector <seq_t> & seq) const noexcept {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.idw, arpa_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем количество документов
			obj->weight = item.weight;
			// Запоминаем встречаемость так-как она есть
			obj->backoff = item.backoff;
			// Если регистры слова переданы, считаем их
			if(item.ups > 0) obj->uppers = item.ups;
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
}
/**
 * set Метод установки последовательности в словарь
 * @param seq     список идентификаторов слов которые нужно добавить
 * @param weight  вес n-граммы из файла arpa
 * @param backoff обратная частота документа из файла arpa
 */
void anyks::Alm::set(const vector <pair_t> & seq, const float weight, const float backoff) const noexcept {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.first, arpa_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем регистры слова
			obj->uppers = item.second;
			// Если мы дошли до конца
			if(i == (seq.size() - 1)){
				// Запоминаем обратную частоту документа
				obj->backoff = (backoff == 0.0f ? this->zero : backoff);
				// Запоминаем частоту n-граммы
				obj->weight = ((weight == 0.0f) || (fabs(round(weight)) >= 99.0f) ? this->zero : weight);
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
}
/**
 * clearShielding Функция удаления экранирования
 * @param word  слово в котором следует удалить экранирование
 * @param front первый символ экранирования
 * @param back  последний символ экранирования
 */
void anyks::Alm::clearShielding(const string & word, const string & front, const string & back) const noexcept {
	// Если слово передано
	if(!word.empty() && (!front.empty() || !back.empty())){
		// Избавляемся от модификатора const
		string & str = const_cast <string &> (word);
		// Если первый символ экранирования передан
		if(!front.empty() && (str.substr(0, front.length()).compare(front) == 0)){
			// Удаляем первый символ экранирования
			str.replace(0, front.length(), "");
		}
		// Если второй символ экранирования передан
		if(!back.empty() && (str.substr(str.length() - back.length(), back.length()).compare(back) == 0)){
			// Удаляем первый символ экранирования
			str.replace(str.length() - back.length(), back.length(), "");
		}
	}
}
/**
 * weight Метод извлечения веса последовательности
 * @param seq последовательность для извлечения веса
 * @return    вес последовательноси и n-грамма для которой она получена
 */
const pair <u_short, float> anyks::Alm::weight(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	pair <u_short, float> result = {0, 0.0f};
	// Если контекст передан
	if(!seq.empty() && !this->arpa.empty()){
		// Флаг полностью собранной последовательности
		bool isFull = true;
		// Копируем основную карту
		const arpa_t * obj = &this->arpa;
		// Временная последовательность
		vector <size_t> tmp(seq.begin() + 1, seq.end());
		// Переходим по всей последовательности
		for(auto & idw : tmp){
			// Ищем нашу n-грамму
			auto it = obj->find(idw);
			// Если n-грамма найдена
			if(it != obj->end()) obj = &it->second;
			// Запоминаем что последовательность не собрана
			else {
				// Запоминаем что последовательность не существует
				isFull = false;
				// Выходим из цикла
				break;
			}
		}
		// Если последовательность существует, получаем её вес
		if(isFull && (obj->weight != this->zero))
			// Формируем полученный вес n-граммы
			result = make_pair((u_short) tmp.size(), obj->weight);
		// Иначе продолжаем дальше
		else result = this->weight(tmp);
	}
	// Выводим результат
	return result;
}
/**
 * frequency Метод извлечения частоты n-граммы
 * @param seq список слов последовательности
 * @return    частота и обратная частота n-граммы
 */
const pair <float, float> anyks::Alm::frequency(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	pair <float, float> result = {this->zero, this->zero};
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0) && !this->arpa.empty()){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & idw : seq){
			// Выполняем поиск нашего слова
			auto it = obj->find(idw);
			// Если слово найдено
			if(it != obj->end()){
				// Получаем блок структуры
				obj = &it->second;
				// Если мы дошли до конца
				if(i == (seq.size() - 1)) result = make_pair(obj->weight, obj->backoff);
			// Выходим из цикла
			} else break;
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
	// Выводим результат
	return result;
}
/**
 * pplCalculate Метод расчёта значения перплексии по весу
 * @param logprob вес последовательности n-грамм полученный из расчётов
 * @param wirds   список слов используемых при расчёте
 * @param oovs    список неизвестных слов используемых при расчётах
 * @return        значение перплексии полученное при расчётах
 */
const pair <float, float> anyks::Alm::pplCalculate(const float logprob, const size_t words, const size_t oovs) const noexcept {
	// Результат работы функции
	pair <float, float> result = {this->zero, this->zero};
	// Если вес переданный верный
	if((logprob != 0.0f) && (logprob != this->zero) && (words > 0)){
		// Выполняем расчёт перплексии
		result.second = exp(- (logprob / float(words - oovs)) * this->mln10);
		result.first  = exp(- (logprob / float(words - oovs + 1)) * this->mln10);
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Alm::isOption(const options_t option) const noexcept {
	// Выполняем проверку наличия опции
	return this->options.test((u_short) option);
}
/**
 * backoff Метод извлечения обратной частоты последовательности
 * @param seq последовательность для извлечения обратной частоты
 * @return    обратная частота последовательности
 */
const float anyks::Alm::backoff(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	float result = 0.0f;
	// Если контекст передан
	if(!seq.empty() && !this->arpa.empty()){
		// Идентификатор текущего слова
		size_t idw = 0;
		// Флаг полностью собранной последовательности
		bool isFull = true;
		// Временная последовательность
		vector <size_t> tmp;
		// Копируем основную карту
		const arpa_t * obj = &this->arpa;
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Получаем идентификатор текущего слова
			idw = seq.at(i);
			// Если последовательность пока собирается удачно
			if(isFull){
				// Ищем нашу n-грамму
				auto it = obj->find(idw);
				// Если n-грамма найдена
				if(it != obj->end()) obj = &it->second;
				// Запоминаем что последовательность не собрана
				else isFull = false;
			}
			// Собираем последовательность для дальнейшего поиска
			if(i > 0) tmp.push_back(idw);
		}
		// Если последовательность собрана, запоминаем её
		if(isFull && (obj->backoff != this->zero)) result = obj->backoff;
		// Если нужно продолжить сбор дальше
		if(!tmp.empty()) result += this->backoff(tmp);
	}
	// Выводим результат
	return result;
}
/**
 * getIdws Метод генерирования идентификатора слова
 * @param  word  слово для генерации
 * @param  check нужно выполнить дополнительную проверку слова
 * @return       идентификатор слова
 */
const size_t anyks::Alm::getIdw(const wstring & word, const bool check) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!word.empty()){
		// Проверяем является ли слово, началом предложения
		if(word.compare(L"<s>") == 0) result = (size_t) token_t::start;
		// Проверяем является ли слово числом
		else if(word.compare(L"<num>") == 0) result = (size_t) token_t::num;
		// Проверяем является ли слово неизвестным
		else if(word.compare(L"<unk>") == 0) result = (size_t) token_t::unk;
		// Проверяем является ли слово url адресом
		else if(word.compare(L"<url>") == 0) result = (size_t) token_t::url;
		// Проверяем является ли слово аббревиатурой
		else if(word.compare(L"<abbr>") == 0) result = (size_t) token_t::abbr;
		// Проверяем является ли слово датой
		else if(word.compare(L"<date>") == 0) result = (size_t) token_t::date;
		// Проверяем является ли слово временем
		else if(word.compare(L"<time>") == 0) result = (size_t) token_t::time;
		// Проверяем является ли слово псевдо-числом
		else if(word.compare(L"<anum>") == 0) result = (size_t) token_t::anum;
		// Проверяем является ли слово концом предложения
		else if(word.compare(L"</s>") == 0) result = (size_t) token_t::finish;
		// Проверяем является ли слово математической операцией
		else if(word.compare(L"<math>") == 0) result = (size_t) token_t::math;
		// Проверяем является ли слово спец-символом
		else if(word.compare(L"<specl>") == 0) result = (size_t) token_t::specl;
		// Проверяем является ли слово диапазоном чисел
		else if(word.compare(L"<range>") == 0) result = (size_t) token_t::range;
		// Проверяем является ли слово знаком пунктуации
		else if(word.compare(L"<punct>") == 0) result = (size_t) token_t::punct;
		// Проверяем является ли слово приблизительным числом
		else if(word.compare(L"<aprox>") == 0) result = (size_t) token_t::aprox;
		// Проверяем является ли слово числовым счётом
		else if(word.compare(L"<score>") == 0) result = (size_t) token_t::score;
		// Проверяем является ли слово габаритными размерами
		else if(word.compare(L"<dimen>") == 0) result = (size_t) token_t::dimen;
		// Проверяем является ли слово числовой дробью
		else if(word.compare(L"<fract>") == 0) result = (size_t) token_t::fract;
		// Проверяем является ли слово знаком изоляции
		else if(word.compare(L"<isolat>") == 0) result = (size_t) token_t::isolat;
		// Если это другое слово
		else {
			// Формируем идентификатор слова
			result = this->tokenizer->idw(word);
			// Если нужно выполнить проверку слов
			if(check){
				// Проверяем является ли слово хорошим
				if(this->goodwords.empty() || (this->goodwords.count(result) < 1)){
					// Получаем временное слово
					wstring tmp = word;
					// Подсчитываем количество дефисов
					u_short hyphenCounts = 0;
					// Если нужно проверить пользовательские токены
					if(!this->utokens.empty()){
						// Переходим по всему списку пользовательских токенов
						for(auto & token : this->utokens){
							// Если сработал пользовательский токен
							if(token.second.test(
								token.second.name.real(),
								this->alphabet->convert(tmp)
							)) return token.first;
						}
					}
					// Проверяем является ли слово знаком пунктуации
					if(tmp.length() == 1){
						// Получаем букву для проверки
						const wchar_t letter = tmp.front();
						// Если это знак пунктуации
						if(this->alphabet->isPunct(letter)){
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::punct) < 1) result = (size_t) token_t::punct;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::punct) > 0) result = (size_t) token_t::unk;
						// Если буква является арабским числом
						} else if(this->alphabet->isNumber({letter})) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::num) < 1) result = (size_t) token_t::num;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::num) > 0) result = (size_t) token_t::unk;
						// Если это символ математической операции
						} else if(this->alphabet->isMath(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::math) < 1) result = (size_t) token_t::math;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::math) > 0) result = (size_t) token_t::unk;
						// Если это спец-символ
						} else if(this->alphabet->isSpecial(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::specl) < 1) result = (size_t) token_t::specl;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::specl) > 0) result = (size_t) token_t::unk;
						// Если это знак изоляции
						} else if(this->alphabet->isIsolation(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::isolat) < 1) result = (size_t) token_t::isolat;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::isolat) > 0) result = (size_t) token_t::unk;
						}
					// Проверяем есть ли изоляционный знак и количество дефисов в слове больше 2-х
					} else if(
						this->alphabet->isIsolation(tmp.back()) ||
						this->alphabet->isIsolation(tmp.front()) ||
						((hyphenCounts = this->alphabet->countLetter(tmp, L'-')) > 2)
					) result = (size_t) token_t::unk;
					// Если идентификатор определить не удалось
					else {
						// Идентификатор токена слова
						token_t idt = token_t::null;
						// Получаем идентификатор токена слова
						const token_t token = this->tokenizer->idt(tmp);
						// Пытаемся определить идентификатор слова
						switch((u_short) token){
							// Если это токен неизвестного слова
							case (u_short) token_t::unk: idt = token; break;
							// Если это токен числа, запоминаем его
							case (u_short) token_t::num: {
								// Если идентифицирование токена не отключено
								if(this->tokenDisable.count(token) < 1) idt = token;
								// Устанавливаем токен неизвестного слова
								if(this->tokenUnknown.count(token) > 0) idt = token_t::unk;
							} break;
							// Если токен определён как слово
							case (u_short) token_t::null: if(this->isOption(options_t::onlyGood)) idt = token_t::unk; break;
							// Если это другие токены
							default: {
								// Если идентифицирование токена не отключено
								if(this->tokenDisable.count(token) < 1) idt = token;
								// Устанавливаем токен неизвестного слова
								if(this->tokenUnknown.count(token) > 0) idt = token_t::unk;
							}
						}
						// Если слово определено как число но это не число, значит это римское число
						if((idt == token_t::num) && !this->alphabet->isNumber({tmp.back()})){
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::rnum) < 1) result = (size_t) token_t::num;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::rnum) > 0) result = (size_t) token_t::unk;
						// Иначе запоминаем идентификатор так-как он передан
						} else if(idt != token_t::null) result = (size_t) idt;
						// Если разрешено детектировать слова из смешанных словарей
						else if(this->isOption(options_t::mixdicts)){
							// Пытаемся детектировать слово со смешанными буквами
							if(this->alphabet->rest(tmp)) result = (size_t) token_t::unk;
							// Если слова отличаются, получаем новый идентификатор
							else if(tmp.compare(word) != 0) result = this->tokenizer->idw(tmp);
						}
					}
				}
			}
		}
	}
	// Если результат не определён, возвращаем неизвестное слово
	result = (result > 0 ? result : (size_t) token_t::unk);
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии текста
 * @param  text текст для расчёта
 * @return      результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const string & text) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!text.empty()) result = this->perplexity(this->alphabet->convert(text));
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии текста
 * @param  text текст для расчёта
 * @return      результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const wstring & text) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!text.empty() && !this->arpa.empty()){
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Идентификатор начала предложения
		const size_t sid = (size_t) token_t::start;
		// Идентификатор конца предложения
		const size_t fid = (size_t) token_t::finish;
		// Список последовательностей для обучения
		vector <size_t> seq = {sid};
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, this]() noexcept {
			// Если неизвестное слово не установлено
			seq.push_back(uid);
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, fid, sid, this]() noexcept {
			// Добавляем в список конец предложения
			seq.push_back(fid);
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2){
				// Выводим отладочную информацию
				if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
					// Получаем обрабатываемый текст
					const wstring & text = this->context(seq, true);
					// Выводим сообщение отладки - количество слов
					this->alphabet->log("%s\r\n", alphabet_t::log_t::info, this->logfile, this->alphabet->convert(text).c_str());
				}
				// Выполняем расчёт перплексии нашей текстовой последовательности
				result = (result.words == 0 ? this->perplexity(seq) : this->pplConcatenate(result, this->perplexity(seq)));
			}
			// Очищаем список последовательностей
			seq.clear();
			// Добавляем в список начало предложения
			seq.push_back(sid);
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & word, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если это сброс контекста, отправляем результат
			if(reset) resFn();
			// Если слово передано
			if(!word.empty()){
				// Получаем данные слова
				word_t tmp = word;
				// Если модуль питона активирован
				if(this->python != nullptr){
					// Ищем скрипт обработки слов
					auto it = this->scripts.find(1);
					// Если скрипт обработки слов установлен
					if(it != this->scripts.end()){
						// Блокируем поток
						this->locker.lock();
						// Выполняем внешний python скрипт
						tmp = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат не получен, возвращаем слово
						if(tmp.empty()) tmp = move(word);
						// Разблокируем поток
						this->locker.unlock();
					}
				}
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if((idw == 0) || (idw == idw_t::NIDW) || (this->badwords.count(idw) > 0)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = this->event(idw);
						// Если это неизвестное слово
						if((idw == uid) || (isWord && (this->findWord(idw) == nullptr))) unkFn(); 
						// Иначе добавляем слово
						else if(!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)) seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn();
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
		// Выводим отладочную информацию
		if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
			// Выводим разделитель
			cout << endl;
			// Выводим сообщение отладки - количество слов
			this->alphabet->log(
				"%u sentences, %u words, %u OOVs",
				alphabet_t::log_t::info,
				this->logfile,
				result.sentences,
				result.words,
				result.oovs
			);
			// Выводим сообщение отладки - результатов расчёта
			this->alphabet->log(
				"%u zeroprobs, logprob= %4.6f ppl= %4.6f ppl1= %4.6f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				result.zeroprobs,
				result.logprob,
				result.ppl,
				result.ppl1
			);
		}
	}
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии
 * @param  seq список последовательностей
 * @return     результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!this->arpa.empty() && (seq.size() > 2) && (this->size > 0) && (seq.front() == size_t(token_t::start)) && (seq.back() == size_t(token_t::finish))){
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Если последовательность в виде юниграммы
		if(count == 3){
			// Получаем идентификатор слова
			const size_t idw = seq.at(1);
			// Выполняем поиск нашего слова
			auto it = this->arpa.find(idw);
			// Если слово найдено
			if(it != this->arpa.end()){
				// Извлекаем вес слова
				result.logprob = it->second.weight;
			// Устанавливаем количество неизвестных слов
			} else if(idw == size_t(token_t::unk)) result.oovs++;
		// Если это не юниграмма
		} else {
			// Временная последовательность
			vector <size_t> tmp;
			// Определяем смещение в последовательности
			size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
			// Проверяем разрешено ли неизвестное слово
			const bool isAllowUnk = (
				(this->arpa.count(size_t(token_t::unk)) > 0) &&
				(this->arpa.at(size_t(token_t::unk)).weight != this->zero)
			);
			/**
			 * debugFn Функция вывода отладочной информации
			 * @param first  первое слово
			 * @param second второе слово
			 * @param bigram является ли n-грамма длиннее биграммы
			 * @param gram   граммность n-граммы для которой был произведён расчёт
			 * @param weight полученный вес n-граммы при расчёте
			 * @param delim  проверочный делитель n-граммы
			 */
			auto debugFn = [this](const string & first, const string & second, const bool bigram, const u_short gram, const float weight, const float delim){
				// Выводим отладочную информацию
				if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
					// Граммность n-граммы
					string numGram = "OOV";
					// Значение полученного веса
					float prob = 0.0f, lprob = this->zero;
					// Если вес не нулевой
					if(weight != 0.0f){
						// Запоминаем вес n-граммы
						lprob = weight;
						// Избавляемся от логорифма
						prob = pow(10, weight);
						// Устанавливаем граммность
						numGram = (to_string(gram) + "gram");
					}
					// Выводим сообщение отладки
					this->alphabet->log(
						"p( %s | %s %s) \t= [%s] %4.6f [ %4.6f ] / %4.6f",
						alphabet_t::log_t::info,
						this->logfile,
						second.c_str(),
						first.c_str(),
						(bigram ? "..." : ""),
						numGram.c_str(),
						prob, lprob, delim
					);
					// Выполняем округление делителя
					const double value = (ceil((delim * 10000.0) + 0.5) / 10000.0);
					// Если делитель не сходится к единице, выводим сообщение
					if(fabs(value - 1.0) > 0.0009){
						// Выводим сообщение отладки
						this->alphabet->log("word probs for this context sum to %4.6f != 1", alphabet_t::log_t::warning, this->logfile, delim);
					}
				}
			};
			/**
			* calcFn Функция расчёта перплексии
			* @return вес n-граммы
			*/
			auto calcFn = [&tmp, isAllowUnk, this]() noexcept {
				// Результат работы функции
				pair <u_short, float> result = {0, 0.0f};
				// Если данные не пустые
				if(!tmp.empty()){
					// Получаем нашу последовательность
					vector <size_t> seq = tmp;
					// Если первый элемент является неизвестным словом, удаляем его
					if(!isAllowUnk && (seq.front() == size_t(token_t::unk))){
						// Удаляем первый элемент в списке
						seq.assign(seq.begin() + 1, seq.end());
					}
					// Если есть еще смысл искать
					if(!seq.empty()){
						// Копируем основную карту
						const arpa_t * obj = &this->arpa;
						// Переходим по всей последовательностив
						for(auto & idw : seq){
							// Ищем нашу n-грамму
							auto it = obj->find(idw);
							// Если n-грамма найдена
							if(it != obj->end()){
								// Запоминаем следующую n-грамму
								obj = &it->second;
								// Запоминаем вес n-граммы
								if(obj->weight != this->zero) result.second = obj->weight;
							// Если n-грамма не найдена
							} else {
								// Если последнее слово последовательности найдено
								if(this->arpa.count(seq.back()) > 0){
									// Получаем вес последовательности
									const auto wrs = this->weight(seq);
									// Получаем грамность
									result.first = wrs.first;
									// Запоминаем полученный вес
									result.second = wrs.second;
									// Если вес получен для юниграммы, выполняем поиск частоты отката
									if(result.first == 1){
										// Получаем список последовательности для извлечения обратной частоты
										seq.assign(seq.begin(), seq.end() - 1);
										// Выполняем расчёт веса n-граммы
										result.second += this->backoff(seq);
									}
								// Если слово не найдено, устанавливаем -inf
								} else result.second = 0.0f;
								// Выходим из цикла
								break;
							}
							// Увеличиваем граммность
							result.first++;
						}
					}
				}
				// Выводим результат
				return result;
			};
			/**
			 * putDebugFn Функция расчёта отладочной информации
			 * @param gram   граммность n-граммы для которой был произведён расчёт
			 * @param weight полученный вес n-граммы при расчёте
			 */
			auto putDebugFn = [&debugFn, &tmp, this](const u_short gram, const float weight) noexcept {
				// Если последовательность передана
				if(!tmp.empty() && this->isOption(options_t::debug)){
					// Получившийся разделитель
					float delim = 0.0f;
					// Получаем количество слов в последовательности
					const size_t count = tmp.size();
					// Выполняем првоерку больше ли переданная последовательность биграммы
					const bool isBigram = (count > 2);
					// Получаем второе слово
					const string & second = this->word(tmp.back()).real();
					// Получаем первое слово
					const string & first = this->word(tmp.at(count - 2)).real();
					// Укорачиваем последовательность до 2-х слов
					if(count > 2) tmp.assign(tmp.begin() + (count - 2), tmp.end());
					// Удаляем последний элемент в списке
					tmp.pop_back();
					// Выполняем расчёт обратной частоты последовательности
					const float backoff = this->backoff(tmp);
					// Переходим по всем словам словаря
					for(auto & value : this->arpa){
						// Если веса у n-граммы нету
						if((value.second.weight != this->zero)){
							// Формируем нашу последовательность
							tmp.push_back(value.first);
							// Получаем частоту последовательности
							auto calc = this->frequency(tmp);
							// Если частота последовательности получена
							if(calc.first != this->zero) delim += exp(calc.first * this->mln10);
							// Если последовательность не существует, считаем частоту иначе
							else delim += exp((this->weight(tmp).second + backoff) * this->mln10);
							// Удаляем последний элемент в списке
							tmp.pop_back();
						}
					}
					// Выводим отладочную информацию
					debugFn(first, second, isBigram, gram, weight, delim);
				}
			};
			// Сбрасываем значение результата
			result.logprob = 0.0f;
			// Обрабатываем первую часть n-грамм
			for(u_short i = 2; i < offset2; i++){
				// Получаем первую часть последовательности
				tmp.assign(seq.begin(), seq.begin() + i);
				// Выполняем проверку существования граммы
				auto calc = calcFn();
				// Если вес получен
				if(calc.second != 0.0f)
					// Увеличиваем общее значение веса
					result.logprob += calc.second;
				// Увеличиваем количество нулевых весов
				else result.zeroprobs++;
				// Выводим отладочную информацию
				putDebugFn(calc.first, calc.second);
			}
			// Если есть ещё n-граммы
			if(count >= this->size){
				// Выполняем извлечение данных
				while(offset2 < (count + 1)){
					// Получаем первую часть последовательности
					tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
					// Выполняем проверку существования граммы
					auto calc = calcFn();
					// Если вес получен
					if(calc.second != 0.0f)
						// Увеличиваем общее значение веса
						result.logprob += calc.second;
					// Увеличиваем количество нулевых весов
					else result.zeroprobs++;
					// Выводим отладочную информацию
					putDebugFn(calc.first, calc.second);
					// Увеличиваем смещение
					offset1++;
					offset2++;
				}
			}
			// Если неизвестное слово не разрешено
			if(!isAllowUnk){
				// Считаем количество неизвестных слов
				for(auto & idw : seq){
					// Считаем количество неизвестных слов
					if(idw == size_t(token_t::unk)) result.oovs++;
				}
			}
		}
		// Устанавливаем предложение
		result.sentences = 1;
		// Устанавливаем количество слов
		result.words = (seq.size() - 2);
		// Если количество нулевых весов и количество неизвестных слов получено
		if((result.oovs > 0) && (result.zeroprobs > 0)) result.zeroprobs -= result.oovs;
		// Выполняем расчёт перплексии
		const auto ppl = this->pplCalculate(result.logprob, result.words, result.oovs);
		// Усталавниваем полученные значения перплексии
		result.ppl  = ppl.first;
		result.ppl1 = ppl.second;
		// Выводим отладочную информацию
		if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
			// Выводим разделитель
			cout << endl;
			// Выводим сообщение отладки - количество слов
			this->alphabet->log(
				"%u sentences, %u words, %u OOVs",
				alphabet_t::log_t::info,
				this->logfile,
				result.sentences,
				result.words,
				result.oovs
			);
			// Выводим сообщение отладки - результатов расчёта
			this->alphabet->log(
				"%u zeroprobs, logprob= %4.6f ppl= %4.6f ppl1= %4.6f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				result.zeroprobs,
				result.logprob,
				result.ppl,
				result.ppl1
			);
		}
	}
	// Выводим результат
	return result;
}
/**
 * pplConcatenate Метод объединения перплексий
 * @param ppl1 первая перплексия
 * @param ppl2 вторая перплексия
 * @return     объединённая перплексия
 */
const anyks::Alm::ppl_t anyks::Alm::pplConcatenate(const ppl_t & ppl1, const ppl_t & ppl2) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если перплексии переданы
	if((ppl1.words > 0) && (ppl2.words > 0)){
		// Выполняем сложение двух перплексий
		result.oovs      = (ppl1.oovs + ppl2.oovs);
		result.words     = (ppl1.words + ppl2.words);
		result.logprob   = (ppl1.logprob + ppl2.logprob);
		result.sentences = (ppl1.sentences + ppl2.sentences);
		result.zeroprobs = (ppl1.zeroprobs + ppl2.zeroprobs);
		// Выполняем расчёт перплексии
		const auto ppl = this->pplCalculate(result.logprob, result.words, result.oovs);
		// Усталавниваем полученные значения перплексии
		result.ppl  = ppl.first;
		result.ppl1 = ppl.second;
	}
	// Выводим результат
	return result;
}
/**
 * pplByFiles Метод чтения расчёта перплексии по файлу или группе файлов
 * @param path   адрес каталога или файла для расчёта перплексии
 * @param status функция вывода статуса чтения
 * @param ext    расширение файлов в каталоге (если адрес передан каталога)
 * @return       результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::pplByFiles(const string & path, function <void (const u_short)> status, const string & ext) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если адрес файла передан
	if(!path.empty() && !this->arpa.empty()){
		// Параметры индикаторы процесса
		size_t size = 0, actual = 0, rate = 0;
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем загрузку данных текстового файла
			fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
				// Если текст получен
				if(!text.empty()){
					// Выполняем расчёт перплексии нашей текстовой последовательности
					result = (result.words == 0 ? this->perplexity(text) : this->pplConcatenate(result, this->perplexity(text)));
				}
				// Общий полученный размер данных
				size += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(size / float(fileSize) * 100.0f);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим статистику
					status(actual);
				}
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
				// Выполняем загрузку данных текстового файла
				fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!text.empty()){
						// Выполняем расчёт перплексии нашей текстовой последовательности
						result = (result.words == 0 ? this->perplexity(text) : this->pplConcatenate(result, this->perplexity(text)));
					}
					// Общий полученный размер данных
					size += text.size();
					// Подсчитываем статус выполнения
					actual = u_short(size / float(dirSize) * 100.0f);
					// Если процентное соотношение изменилось
					if(rate != actual){
						// Запоминаем текущее процентное соотношение
						rate = actual;
						// Выводим статистику
						status(actual);
					}
				});
			});
		}
		// Выводим отладочную информацию
		if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
			// Выводим разделитель
			cout << endl;
			// Выводим сообщение отладки - количество слов
			this->alphabet->log(
				"%u sentences, %u words, %u OOVs",
				alphabet_t::log_t::info,
				this->logfile,
				result.sentences,
				result.words,
				result.oovs
			);
			// Выводим сообщение отладки - результатов расчёта
			this->alphabet->log(
				"%u zeroprobs, logprob= %4.6f ppl= %4.6f ppl1= %4.6f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				result.zeroprobs,
				result.logprob,
				result.ppl,
				result.ppl1
			);
		}
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param text текст для проверки существования
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const string & text) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty()) result = this->check(this->alphabet->convert(text));
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param text текст для проверки существования
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const wstring & text) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty() && !this->arpa.empty()){
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, this]() noexcept {
			// Если неизвестное слово не установлено
			seq.push_back(uid);
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, this]() noexcept {
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) result = this->check(seq);
			// Очищаем список последовательностей
			seq.clear();
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & word, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если это сброс контекста, отправляем результат
			if(reset) resFn();
			// Если слово передано
			if(!word.empty()){
				// Получаем данные слова
				word_t tmp = word;
				// Если модуль питона активирован
				if(this->python != nullptr){
					// Ищем скрипт обработки слов
					auto it = this->scripts.find(1);
					// Если скрипт обработки слов установлен
					if(it != this->scripts.end()){
						// Блокируем поток
						this->locker.lock();
						// Выполняем внешний python скрипт
						tmp = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат не получен, возвращаем слово
						if(tmp.empty()) tmp = move(word);
						// Разблокируем поток
						this->locker.unlock();
					}
				}
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if((idw == 0) || (idw == idw_t::NIDW) || (this->badwords.count(idw) > 0)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = this->event(idw);
						// Если это неизвестное слово
						if((idw == uid) || (isWord && (this->findWord(idw) == nullptr))) unkFn();
						// Иначе добавляем слово
						else if(!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)) seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn();
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param seq список слов последовательности
 * @return    результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Временная последовательность
		vector <size_t> tmp, sequence;
		// Если последовательность не экранированна
		if((seq.back() == size_t(token_t::finish)) &&
		(seq.front() == size_t(token_t::start))) sequence.assign(seq.begin() + 1, seq.end() - 1);
		else if(seq.back() == size_t(token_t::finish)) sequence.assign(seq.begin() + 1, seq.end());
		else if(seq.front() == size_t(token_t::start)) sequence.assign(seq.begin(), seq.end() - 1);
		else sequence.assign(seq.begin(), seq.end());
		/**
		 * checkFn Прототип функции проверки существования последовательности
		 * @param  список слов последовательности
		 * @return результат проверки
		 */
		function <const pair <bool, size_t> (const vector <size_t> &)> checkFn;
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq список слов последовательности
		 * @return    результат проверки
		 */
		checkFn = [&checkFn, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			pair <bool, size_t> result = {false, 0};
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						result.first = (i == (seq.size() - 1));
						// Если последовательность существует
						if(result.first) result.second = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if((i < seq.size()) && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = checkFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				result = checkFn(tmp);
				// Если последовательность не найдена, выходим
				if(!result.first) break;
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * fixUppers Метод исправления регистров в тексте
 * @param text текст для исправления регистров
 * @return     текст с исправленными регистрами слов
 */
const string anyks::Alm::fixUppers(const string & text) const noexcept {
	// Результат работы функции
	string result = "";
	// Если текст передан
	if(!text.empty()) result = this->alphabet->convert(this->fixUppers(this->alphabet->convert(text)));
	// Выводим результат
	return result;
}
/**
 * fixUppers Метод исправления регистров в тексте
 * @param text текст для исправления регистров
 * @return     текст с исправленными регистрами слов
 */
const wstring anyks::Alm::fixUppers(const wstring & text) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если текст передан
	if(!text.empty() && !this->arpa.empty()){
		// Флаг первой итерации
		bool start = false;
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * resFn Функция вывода результата
		 * @param word слово которое нужно вставить как есть
		 * @param idw  идентификатор слова если надо
		 */
		auto resFn = [&result, &start, &seq, this](const word_t & word = L"", const size_t idw = 0) noexcept {
			// Выполняем исправление регистров текста
			if(!seq.empty()){
				// Получаем текст последовательности
				wstring text = this->context(seq);
				// Если дополнительное слово не передано, меняем регистр у первой буквы
				if(start && !word.empty() && (word.front() != L'!') &&
				(word.front() != L'?') && (word.front() != L'.')) text.at(0) = towlower(text.front());
				// Добавляем исправленный текст
				result.append(text);
				// Если слово передано
				if(!word.empty()){
					// Добавляем разделитель
					if(idw != size_t(token_t::punct)) result.append(L" ");
					// Добавляем переданное слово
					result.append(word.wreal());
				}
				// Добавляем разделитель
				result.append(L" ");
			// Если слово передано
			} else if(!word.empty()){
				// Добавляем разделитель
				if((idw != size_t(token_t::punct)) &&
				(result.back() != L' ')) result.append(L" ");
				else if((idw == size_t(token_t::punct)) &&
				(result.back() == L' ')) result.pop_back();
				// Добавляем переданное слово
				result.append(word.wreal());
				// Добавляем разделитель
				result.append(L" ");
			}
			// Очищаем список последовательностей
			seq.clear();
			// Запоминаем что первая итерация выполнена
			start = true;
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & word, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если это сброс контекста, отправляем результат
			if(reset) resFn();
			// Если слово передано
			if(!word.empty()){
				// Получаем данные слова
				word_t tmp = word;
				// Если модуль питона активирован
				if(this->python != nullptr){
					// Ищем скрипт обработки слов
					auto it = this->scripts.find(1);
					// Если скрипт обработки слов установлен
					if(it != this->scripts.end()){
						// Блокируем поток
						this->locker.lock();
						// Выполняем внешний python скрипт
						tmp = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат не получен, возвращаем слово
						if(tmp.empty()) tmp = move(word);
						// Разблокируем поток
						this->locker.unlock();
					}
				}
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) resFn(tmp);
				// Если слово разрешено
				else {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if((idw == 0) || (idw == idw_t::NIDW) || (this->badwords.count(idw) > 0)) resFn(tmp);
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = this->event(idw);
						// Если это неизвестное слово
						if((idw == uid) || (isWord && (this->findWord(idw) == nullptr))) resFn(tmp, idw);
						// Иначе добавляем слово
						else if(isWord && this->alphabet->isAllowed(tmp)) seq.push_back(idw);
						// Отправляем слово как неизвестное
						else resFn(tmp, idw);
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
		// Если результат получен, удаляем последний пробел
		if(!result.empty()) result.pop_back();
	}
	// Выводим результат
	return result;
}
/**
 * context Метод сборки текстового контекста из последовательности
 * @param seq  последовательность слов для сборки контекста
 * @param nwrd флаг разрешающий вывод системных токенов
 * @return     собранный текстовый контекст
 */
const wstring anyks::Alm::context(const vector <size_t> & seq, const bool nwrd) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Временная последовательность
		vector <size_t> tmp, sequence = seq;
		// Если последовательность не экранированна
		if(seq.back() != size_t(token_t::finish)) sequence.push_back((size_t) token_t::finish);
		if(seq.front() != size_t(token_t::start)) sequence.insert(sequence.begin(), (size_t) token_t::start);
		/**
		 * uppersFn Прототип функции извлечения регистров последовательности
		 * @param  список слов последовательности
		 * @return регистры последнего слова последовательности
		 */
		function <const size_t (const vector <size_t> &)> uppersFn;
		/**
		 * uppersFn Функция извлечения регистров последовательности
		 * @param seq список слов последовательности
		 * @return    регистры последнего слова последовательности
		 */
		uppersFn = [&uppersFn, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			size_t result = 0;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						if(i == (seq.size() - 1)) result = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if((i < seq.size()) && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = uppersFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Полученное слово последовательности
		word_t word = L"";
		// Регистр слова
		size_t uppers = 0;
		// Флаг сборки первой итерации
		bool flag = false;
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				uppers = uppersFn(tmp);
				// Если сборка первой n-граммы не выполнена
				if(!flag && (flag = !flag)){
					// Переходим по всей последовательности
					for(size_t i = 0; i < tmp.size(); i++){
						// Получаем слово
						word = this->word(tmp.at(i));
						// Если разрешено выводить системные токены или это нормальные слова
						if(nwrd || ((word.front() != L'<') && (word.back() != L'>'))){
							// Если это первое слово
							if(i == 1) word.setUppers(1);
							// Если это последнее слово
							else if(i == (tmp.size() - 1)) word.setUppers(uppers);
							// Формируем строку
							result.append(word.wreal());
							// Добавляем разделитель
							result.append(L" ");
						}
					}
				// Если же сборка первой n-граммы уже выполнена
				} else {
					// Получаем слово
					word = this->word(tmp.back());
					// Если разрешено выводить системные токены или это нормальные слова
					if(nwrd || ((word.front() != L'<') && (word.back() != L'>'))){
						// Устанавливаем регистры слова
						word.setUppers(uppers);
						// Формируем строку
						result.append(word.wreal());
						// Добавляем разделитель
						result.append(L" ");
					}
				}
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
		// Если строка получена
		if(!result.empty()) result.pop_back();
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки
 */
void anyks::Alm::clear(){
	// Очищаем неизвестное слово
	this->unknown = 0;
	// Очищаем словарь arpa
	this->arpa.clear();
	// Очищаем словарь
	this->vocab.clear();
	// Очищаем список пользовательских токенов
	this->utokens.clear();
	// Очищаем список плохих слов
	this->badwords.clear();
	// Очищаем список хороших слов
	this->goodwords.clear();
	// Очищаем список токенов приводимых к <unk>
	this->tokenUnknown.clear();
	// Очищаем список запрещённых токенов
	this->tokenDisable.clear();
	// Если объект питона установлен внешний
	if(this->notCleanPython){
		// Зануляем объект питона
		this->python = nullptr;
		// Разрешаем очистку объекта
		this->notCleanPython = false;
	}
}
/**
 * initPython Метод инициализации скриптов питона
 */
void anyks::Alm::initPython(){
	// Если скрипт получен
	if(((this->scripts.count(2) > 0) && !this->utokens.empty()) || (this->scripts.count(1) > 0)){
		// Экранируем возможность ошибки памяти
		try {
			// Создаём объект для работы с python
			if(this->python == nullptr) this->python = new python_t(this->tokenizer);
			// Блокируем поток
			this->locker.lock();
			// Если нужно активировать режим отладки
			if(this->isOption(options_t::debug)) this->python->setDebug();
			// Разблокируем поток
			this->locker.unlock();
			// Ищем скрипт обработки слов
			auto it = this->scripts.find(1);
			// Если скрипт обработки слов установлен
			if(it != this->scripts.end()){
				// Блокируем поток
				this->locker.lock();
				// Запоминаем идентификатор скрипта
				it->second.second = this->python->add(it->second.first, 2);
				// Разблокируем поток
				this->locker.unlock();
			}
			// Ищем скрипт обработки пользовательских токенов
			it = this->scripts.find(2);
			// Если скрипт обработки пользовательских токенов установлен
			if((it != this->scripts.end()) && !this->utokens.empty()){
				// Блокируем поток
				this->locker.lock();
				// Выполняем добавление скрипта
				const size_t sid = this->python->add(it->second.first, 2);
				// Разблокируем поток
				this->locker.unlock();
				// Переходим по всему списку пользовательских токенов
				for(auto & token : this->utokens){
					// Добавляем в пользовательский токен функцию проверки
					token.second.test = [sid, this](const string & token, const string & word){
						// Результат работы функции
						bool result = false;
						// Если слово и токен переданы
						if(!token.empty() && !word.empty()){
							// Блокируем поток
							this->locker.lock();
							// Выполняем скрипт
							const wstring & res = this->python->run(sid, {token, word});
							// Проверяем результат
							result = (this->alphabet->toLower(res).compare(L"ok") == 0);
							// Разблокируем поток
							this->locker.unlock();
						}
						// Выводим результат
						return result;
					};
				}
			}
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug) || (this->logfile != nullptr)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init python scripts");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * clearBadwords Метод очистки списка плохих слов
 */
void anyks::Alm::clearBadwords() noexcept {
	// Очищаем список плохих слов
	this->badwords.clear();
}
/**
 * clearGoodwords Метод очистки списка хороших слов
 */
void anyks::Alm::clearGoodwords() noexcept {
	// Очищаем список хороших слов
	this->goodwords.clear();
}
/**
 * clearUserTokens Метод очистки пользовательских токенов
 */
void anyks::Alm::clearUserTokens() noexcept {
	// Очищаем список пользовательских токенов
	this->utokens.clear();
}
/**
 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
 */
void anyks::Alm::setAllTokenUnknown() noexcept {
	// Устанавливаем все токены для идентифицировоания как <unk>
	this->tokenUnknown = {
		token_t::num,
		token_t::url,
		token_t::abbr,
		token_t::date,
		token_t::time,
		token_t::anum,
		token_t::math,
		token_t::rnum,
		token_t::specl,
		token_t::aprox,
		token_t::range,
		token_t::score,
		token_t::dimen,
		token_t::fract,
		token_t::punct,
		token_t::isolat
	};
}
/**
 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
 */
void anyks::Alm::setAllTokenDisable() noexcept {
	// Устанавливаем все токены для отключения
	this->tokenDisable = {
		token_t::num,
		token_t::url,
		token_t::abbr,
		token_t::date,
		token_t::time,
		token_t::anum,
		token_t::math,
		token_t::rnum,
		token_t::specl,
		token_t::aprox,
		token_t::range,
		token_t::score,
		token_t::dimen,
		token_t::fract,
		token_t::punct,
		token_t::isolat
	};
}
/**
 * addBadword Метод добавления идентификатора похого слова в список
 * @param idw идентификатор слова
 */
void anyks::Alm::addBadword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->badwords.emplace(idw);
}
/**
 * addGoodword Метод добавления идентификатора хорошего слова в список
 * @param idw идентификатор слова
 */
void anyks::Alm::addGoodword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->goodwords.emplace(idw);
}
/**
 * addBadword Метод добавления похого слова в список
 * @param word слово для добавления
 */
void anyks::Alm::addBadword(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
		// Добавляем слово в список плохих слов
		this->addBadword(idw);
	}
}
/**
 * addGoodword Метод добавления хорошего слова в список
 * @param word слово для добавления
 */
void anyks::Alm::addGoodword(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
		// Добавляем слово в список хороших слов
		this->addGoodword(idw);
	}
}
/**
 * setWordMethod Метод установки функции получения слова
 * @param word функция получения слова
 */
void anyks::Alm::setWordMethod(words_t word) noexcept {
	// Устанавливаем функцию получения слова
	this->getWord = word;
}
/**
 * setUnknown Метод установки неизвестного слова
 * @param word слово для добавления
 */
void anyks::Alm::setUnknown(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Формируем идентификатор слова
		this->unknown = this->tokenizer->idw(this->alphabet->convert(word));
		// Если идентификатор получен
		if(this->unknown > 0){
			// Если слова нет в словаре, добавляем его
			if(this->vocab.count(this->unknown) < 1)
				// Добавляем слово в список
				this->vocab.emplace(this->unknown, word);
			// Если слово уже существует
			else {
				// Получаем слово из словаря
				word_t * word = &this->vocab.at(this->unknown);
				// Получаем метаданные
				auto meta = word->getmeta();
				// Увеличиваем встречаемость слова
				meta.oc++;
				// Устанавливаем полученные метаданные
				word->setmeta(meta);
			}
		}
	}
}
/**
 * setOptions Метод установки опций
 * @param options опции для установки
 */
void anyks::Alm::setOptions(const u_int options) noexcept {
	// Устанавливаем опции модуля
	this->options = options;
}
/**
 * setPythonObj Метод установки внешнего объекта питона
 * @param python внешний объект питона
 */
void anyks::Alm::setPythonObj(python_t * python) noexcept {
	// Если объект передан
	if(python != nullptr){
		// Устанавливаем объект питона
		this->python = python;
		// Запрещаем очистку объекта
		this->notCleanPython = true;
	}
}
/**
 * setLogfile Метод установка файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Alm::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setUserToken Метод добавления токена пользователя
 * @param name слово - обозначение токена
 */
void anyks::Alm::setUserToken(const string & name) noexcept {
	// Если слово передано
	if(!name.empty()){
		// Создаём новый пользовательский токен
		utoken_t userToken;
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название токена
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			userToken.idw = this->tokenizer->idw(this->alphabet->convert(word));
			// Если такого идентификатора нет в списке
			if((userToken.idw > 0) && (this->utokens.count(userToken.idw) < 1)){
				// Добавляем в пользовательский токен наши параметры
				userToken.name = move(word);
				// Добавляем в список токенов, наш токен
				this->utokens.emplace(userToken.idw, move(userToken));
			}
		}
	}
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Alm::setOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Alm::unsetOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setWordScript Метод установки скрипта обработки слов
 * @param script скрипт python обработки слов
 */
void anyks::Alm::setWordScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(1, make_pair(script, 0));
}
/**
 * setVocab Метод загрузки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Alm::setVocab(const vector <char> & buffer) noexcept {
	// Если буфер слова передан
	if(!buffer.empty()){
		// Идентификатор слова и смещение в буфере
		size_t idw = 0, offset = 0;
		// Извлекаем бинарные данные буфера
		const char * data = buffer.data();
		// Извлекаем идентификатор слова
		memcpy(&idw, data + offset, sizeof(idw));
		// Увеличиваем смещение
		offset += sizeof(idw);
		// Если идентификатор слова получен
		if((idw > 0) && (idw < idw_t::NIDW)){
			// Слово для добавления в словарь
			word_t word = L"";
			// Добавляем бинарные данные слова
			word.set(data + offset, buffer.size() - offset);
			// Добавляем слово в словарь
			if(!word.empty()) this->vocab.emplace(idw, move(word));
		}
	}
}
/**
 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
 * @param options список токенов которых нужно идентифицировать как <unk>
 */
void anyks::Alm::setTokenUnknown(const string & options) noexcept {
	// Если опции переданы
	if(!options.empty()){
		// Список токенов для работы
		vector <wstring> tokens;
		// Выполняем разбиение строки на составляющие
		this->alphabet->split(options, "|", tokens);
		// Если токены получены
		if(!tokens.empty()){
			// Переходим по всему списку токенов
			for(auto & token : tokens){
				// Определяем тип токена
				if(token.compare(L"num") == 0)         this->tokenUnknown.emplace(token_t::num);
				else if(token.compare(L"url") == 0)    this->tokenUnknown.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)   this->tokenUnknown.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)   this->tokenUnknown.emplace(token_t::date);
				else if(token.compare(L"time") == 0)   this->tokenUnknown.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)   this->tokenUnknown.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)   this->tokenUnknown.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)   this->tokenUnknown.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)  this->tokenUnknown.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)  this->tokenUnknown.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)  this->tokenUnknown.emplace(token_t::range);
				else if(token.compare(L"score") == 0)  this->tokenUnknown.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)  this->tokenUnknown.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)  this->tokenUnknown.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)  this->tokenUnknown.emplace(token_t::punct);
				else if(token.compare(L"isolat") == 0) this->tokenUnknown.emplace(token_t::isolat);
			}
		}
	}
}
/**
 * setTokenDisable Метод установки списка не идентифицируемых токенов
 * @param options список не идентифицируемых токенов
 */
void anyks::Alm::setTokenDisable(const string & options) noexcept {
	// Если опции переданы
	if(!options.empty()){
		// Список токенов для работы
		vector <wstring> tokens;
		// Выполняем разбиение строки на составляющие
		this->alphabet->split(options, "|", tokens);
		// Если токены получены
		if(!tokens.empty()){
			// Переходим по всему списку токенов
			for(auto & token : tokens){
				// Определяем тип токена
				if(token.compare(L"num") == 0)         this->tokenDisable.emplace(token_t::num);
				else if(token.compare(L"url") == 0)    this->tokenDisable.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)   this->tokenDisable.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)   this->tokenDisable.emplace(token_t::date);
				else if(token.compare(L"time") == 0)   this->tokenDisable.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)   this->tokenDisable.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)   this->tokenDisable.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)   this->tokenDisable.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)  this->tokenDisable.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)  this->tokenDisable.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)  this->tokenDisable.emplace(token_t::range);
				else if(token.compare(L"score") == 0)  this->tokenDisable.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)  this->tokenDisable.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)  this->tokenDisable.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)  this->tokenDisable.emplace(token_t::punct);
				else if(token.compare(L"isolat") == 0) this->tokenDisable.emplace(token_t::isolat);
			}
		}
	}
}
/**
 * setBin Метод установки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 */
void anyks::Alm::setBin(const vector <char> & buffer) const noexcept {
	// Если буфер передан
	if(!buffer.empty()){
		// Количество слов в последовательности
		u_short count = 0;
		// Смещение в буфере
		size_t offset = 0;
		// Полученные данные последовательности
		vector <seq_t> seq;
		// Получаем данные буфера
		const char * data = buffer.data();
		// Извлекаем количество слов в последовательности
		memcpy(&count, data + offset, sizeof(count));
		// Увеличиваем смещение
		offset += sizeof(count);
		// Если последовательность получена
		if(count > 0){
			// Полученная последовательность
			seq_t sequence;
			// Выделяем память для последовательности
			seq.resize(count);
			// Переходим по всем словам последовательности
			for(u_short i = 0; i < count; i++){
				// Извлекаем данные слова
				memcpy(&sequence, data + offset, sizeof(sequence));
				// Добавляем последовательность в список
				seq[i] = sequence;
				// Увеличиваем смещение
				offset += sizeof(sequence);
			}
			// Если нужно установить исходные данные
			this->set(seq);
		}
	}
}
/**
 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
 * @param script скрипт python обработки пользовательских токенов
 */
void anyks::Alm::setUserTokenScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(2, make_pair(script, 0));
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Alm::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Запоминаем объект алфавита
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Alm::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Если токенизатор передан
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
 * @param tokens список токенов для установки
 */
void anyks::Alm::setTokensUnknown(const std::set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenUnknown = move(tokens);
}
/**
 * setTokensDisable Метод установки списка запрещённых токенов
 * @param tokens список токенов для установки
 */
void anyks::Alm::setTokensDisable(const std::set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenDisable = move(tokens);
}
/**
 * setBadwords Метод установки списка идентификаторов плохих слов в список
 * @param badwords список идентификаторов плохих слов
 */
void anyks::Alm::setBadwords(const std::set <size_t> & badwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!badwords.empty()) this->badwords = move(badwords);
}
/**
 * setBadwords Метод установки списка плохих слов в список
 * @param badwords список плохих слов
 */
void anyks::Alm::setBadwords(const vector <string> & badwords) noexcept {
	// Если список не пустой
	if(!badwords.empty()){
		// Очищаем список плохих слов
		this->badwords.clear();
		// Переходим по всему списку слов
		for(auto & word : badwords) this->addBadword(word);
	}
}
/**
 * setGoodwords Метод установки списка идентификаторов хороших слов в список
 * @param goodwords список идентификаторов хороших слов
 */
void anyks::Alm::setGoodwords(const std::set <size_t> & goodwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!goodwords.empty()) this->goodwords = move(goodwords);
}
/**
 * setGoodwords Метод установки списка хороших слов в список
 * @param goodwords список хороших слов
 */
void anyks::Alm::setGoodwords(const vector <string> & goodwords) noexcept {
	// Если список не пустой
	if(!goodwords.empty()){
		// Очищаем список хороших слов
		this->goodwords.clear();
		// Переходим по всему списку слов
		for(auto & word : goodwords) this->addGoodword(word);
	}
}
/**
 * getUppers Метод извлечения регистров для каждого слова
 * @param seq  последовательность слов для сборки контекста
 * @param upps список извлечённых последовательностей
 */
void anyks::Alm::getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept {
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Очищаем список регистров
		upps.clear();
		// Временная последовательность
		vector <size_t> tmp, sequence = seq;
		// Если последовательность не экранированна
		const bool isFront = (seq.back() == size_t(token_t::finish));
		const bool isBack =  (seq.front() == size_t(token_t::start));
		// Если флаги не установлены
		if(!isFront) sequence.push_back((size_t) token_t::finish);
		if(!isBack)  sequence.insert(sequence.begin(), (size_t) token_t::start);
		/**
		 * uppersFn Прототип функции извлечения регистров последовательности
		 * @param  список слов последовательности
		 * @return регистры последнего слова последовательности
		 */
		function <const size_t (const vector <size_t> &)> uppersFn;
		/**
		 * uppersFn Функция извлечения регистров последовательности
		 * @param seq список слов последовательности
		 * @return    регистры последнего слова последовательности
		 */
		uppersFn = [&uppersFn, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			size_t result = 0;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						if(i == (seq.size() - 1)) result = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if((i < seq.size()) && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = uppersFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Регистр слова
		size_t uppers = 0;
		// Флаг сборки первой итерации
		bool flag = false;
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				uppers = uppersFn(tmp);
				// Если сборка первой n-граммы не выполнена
				if(!flag && (flag = !flag)){
					// Переходим по всей последовательности
					for(size_t i = (!isFront ? 1 : 0); i < tmp.size(); i++){
						// Если это не последнее слово в списке, добавляем нули
						if(i != (tmp.size() - 1))
							upps.push_back(0);
						// Если же это последнее слово, добавляем регистр
						else upps.push_back(uppers);
					}
				// Если же сборка первой n-граммы уже выполнена
				} else upps.push_back(uppers);
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
		// Удаляем лишний элемент регистра
		if(!isBack) upps.pop_back();
	}
}
/**
 * getBin Метод извлечения данных arpa в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Alm::getBin(function <void (const vector <char> &, const u_short)> callback) const noexcept {
	// Если данные загружены
	if(!this->arpa.empty()){
		// Данные последовательности
		seq_t sequence;
		// Индекс обработки статуса
		size_t index = 0;
		// Собранная последовательность
		vector <seq_t> seq;
		// Буфер данных n-граммы
		vector <char> buffer;
		/**
		 * resultFn Метод формирования результата
		 */
		auto resultFn = [&buffer, &seq, &index, &callback, this]() noexcept {
			// Получаем количество n-грамм в списке
			u_short count = seq.size();
			// Получаем бинарные данные количества слов
			const char * bin = reinterpret_cast <const char *> (&count);
			// Добавляем в буфер количество слов
			buffer.insert(buffer.end(), bin, bin + sizeof(count));
			// Переходим по всему списку последовательности
			for(auto & item : seq){
				// Получаем бинарные данные последовательности
				bin = reinterpret_cast <const char *> (&item);
				// Добавляем в буфер бинарные данные последовательности
				buffer.insert(buffer.end(), bin, bin + sizeof(item));
			}
			// Выводим собранную последовательность
			callback(buffer, u_short(index / float(this->arpa.size()) * 100.0f));
			// Очищаем полученный буфер n-граммы
			buffer.clear();
		};
		/**
		 * runFn Прототип функции запуска формирования map карты последовательностей
		 * @param словарь для извлечения слова с его параметрами
		 */
		function <void (const arpa_t *)> runFn;
		/**
		 * runFn Функция запуска формирования map карты последовательностей
		 * @param ngrams словарь для извлечения слова с его параметрами
		 */
		runFn = [&](const arpa_t * ngrams) noexcept {
			// Идентификатор полученного и добавленного слова
			size_t idw = idw_t::NIDW, aidw = 0;
			// Получаем объект данных
			const arpa_t * obj = (ngrams != nullptr ? ngrams : &this->arpa);
			// Переходим по всему объекту с данными
			for(auto & item : * obj){
				// Увеличиваем индекс если это юниграмма
				if(ngrams == nullptr) index++;
				// Запоминаем идентификатор добавленного слова
				idw = sequence.idw;
				// Извлекаем основные данные
				sequence.idw     = item.first;
				sequence.ups     = item.second.uppers;
				sequence.weight  = item.second.weight;
				sequence.backoff = item.second.backoff;
				// Формируем последовательность
				seq.push_back(sequence);
				// Если еще есть продолжение граммы
				if(!item.second.empty()) runFn(&item.second);
				// Иначе выводим то что есть
				else if(idw != aidw) {
					// Запоминаем идентификатор добавленного слова
					aidw = idw;
					// Выводим результат
					resultFn();
				}
				// Удаляем последний элемент в списке
				seq.pop_back();
			}
		};
		// Запускаем извлечение данных
		runFn(nullptr);
	// Выводим пустой результат
	} else callback({}, 0);
}
/**
 * getVocab Метод извлечения данных словаря в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Alm::getVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept {
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Индекс текущего слова
		size_t index = 0;
		// Буфер данных слова
		vector <char> buffer;
		// Переходим по всему списку слов
		for(auto & item : this->vocab){
			// Извлекаем бинарные данные слова
			auto data = item.second.dump();
			// Получаем бинарные данные идентификатора слова
			const char * bin = reinterpret_cast <const char *> (&item.first);
			// Добавляем в буфер идентификатор слова
			buffer.insert(buffer.end(), bin, bin + sizeof(item.first));
			// Добавляем в буфер бинарные данные самого слова
			buffer.insert(buffer.end(), data.begin(), data.end());
			// Выводим результат
			callback(buffer, (index / float(this->vocab.size()) * 100.0f));
			// Очищаем собранный буфер данных
			buffer.clear();
			// Увеличиваем индекс слова
			index++;
		}
	}
}
/**
 * read Метод чтения данных из файла arpa
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса чтения
 */
void anyks::Alm::read(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.compare("\\data\\") == 0) type++; break;
					// Если мы дошли до считывания данных слов
					case 1: if(text.rfind("-grams:") != string::npos) type++; break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.compare("\\end\\") == 0) break;
						// Иначе считываем данные n-граммы
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Данные n-граммы и обратной частоты документа
							string ngram = "", backoff = "0.0";
							// Считываем частоту n-граммы
							const string & weight = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение n-граммы
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные n-граммы
								ngram = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Излвлекаем обратную частоту документа
								backoff = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Извлекаем данные n-граммы
							} else ngram = this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1)));
							// Если данные получены
							if(!ngram.empty() && !weight.empty()){
								// Очищаем последовательность
								seq.clear();
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Идентификатор слова
									size_t idw = 0;
									// Полученное слово
									word_t word = L"";
									// Переходим по всему списку слов
									for(auto & item : words){
										// Получаем слово
										word = move(item);
										// Получаем идентификатор слова
										idw = this->getIdw(word, !this->isOption(options_t::confidence));
										// Если это биграмма
										if((words.size() == 1) && (this->getWord == nullptr)){
											// Проверяем отсутствует ли слово в списке запрещённых слов
											if(this->event(idw) && (this->badwords.count(idw) < 1)) this->vocab.emplace(idw, word);
										}
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(uid == idw){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список последовательности
											} else seq.emplace_back(idw, ((words.size() > 1) && (this->utokens.count(idw) > 0) ? 0 : word.getUppers()));
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Если количество собранных n-грамм выше установленных, меняем
									if(seq.size() > size_t(this->size)) this->size = seq.size();
									// Добавляем последовательность в словарь
									if(!seq.empty()) this->set(seq, stof(weight), stof(backoff));
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / float(fileSize) * 100.0f);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
	// Выводим сообщение об ошибке
	} else if(this->isOption(options_t::debug) || (this->logfile != nullptr)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is not exist");
}
/**
 * setUserTokenMethod Метод добавления функции обработки пользовательского токена
 * @param name слово - обозначение токена
 * @param fn   внешняя функция обрабатывающая пользовательский токен
 */
void anyks::Alm::setUserTokenMethod(const string & name, function <bool (const string &, const string &)> fn) noexcept {
	// Если название токена передано
	if(!name.empty()){
		// Получаем идентификатор токена
		const size_t idw = this->getUserTokenId(name);
		// Ищем нам токен
		auto it = this->utokens.find(idw);
		// Если такой токен найден, устанавливаем функци
		if(it != this->utokens.end()) it->second.test = fn;
	}
}
/**
 * getSize Метод получения размера n-грамы
 * @return длина n-граммы в языковой моделе
 */
const u_short anyks::Alm::getSize() const {
	// Выводим размер n-граммы
	return this->size;
}
/**
 * getOptions Метод извлечения установленных опций
 * @return установленные опции
 */
const u_int anyks::Alm::getOptions() const noexcept {
	// Выводим установленные опции
	return this->options.to_ulong();
}
/**
 * getUnknown Метод извлечения неизвестного слова
 * @return установленное неизвестное слово
 */
const string & anyks::Alm::getUnknown() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если неизвестное слово установлено
	if(this->unknown > 0){
		// Выполняем поиск в словаре
		auto it = this->vocab.find(this->unknown);
		// Если слово найдено
		if(it != this->vocab.end()) const_cast <string *> (&result)->assign(it->second.real());
	}
	// Выводим результат
	return result;
}
/**
 * getWordScript Метод извлечения скрипта обработки слов
 * @return адрес скрипта python обработки слов
 */
const string & anyks::Alm::getWordScript() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(1);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) const_cast <string *> (&result)->assign(it->second.first);
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
 * @return адрес скрипта python обработки пользовательских токенов
 */
const string & anyks::Alm::getUserTokenScript() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(2);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) const_cast <string *> (&result)->assign(it->second.first);
	}
	// Выводим результат
	return result;
}
/**
 * getBadwords Метод извлечения чёрного списка
 * @return чёрный список слов
 */
const std::set <size_t> & anyks::Alm::getBadwords() const noexcept {
	// Выводим результат
	return this->badwords;
}
/**
 * getGoodwords Метод извлечения белого списка
 * @return белый список слов
 */
const std::set <size_t> & anyks::Alm::getGoodwords() const noexcept {
	// Выводим результат
	return this->goodwords;
}
/**
 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
 * @return список токенов
 */
const std::set <anyks::token_t> & anyks::Alm::getTokensUnknown() const noexcept {
	// Выводим список токенов	
	return this->tokenUnknown;
}
/**
 * getTokensDisable Метод извлечения списка запрещённых токенов
 * @return список токенов
 */
const std::set <anyks::token_t> & anyks::Alm::getTokensDisable() const noexcept {
	// Выводим список токенов
	return this->tokenDisable;
}
/**
 * getUserTokens Метод извлечения списка пользовательских токенов
 * @return список пользовательских токенов
 */
const vector <string> & anyks::Alm::getUserTokens() const noexcept {
	// Результат работы функции
	static const vector <string> result;
	// Если пользовательские токены есть
	if(!this->utokens.empty()){
		// Переходим по всем пользовательским токенам
		for(auto & token : this->utokens){
			// Добавляем в список токен слова
			const_cast <vector <string> *> (&result)->push_back(this->getUserTokenWord(token.first));
		}
	}
	// Выводим результат
	return result;
}
/**
 * grams Метод проверки количества найденных в тексте n-грамм
 * @param text текст для расчёта
 * @return     количество найденных n-грамм
 */
const size_t anyks::Alm::grams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty() && !this->arpa.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->grams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * grams Метод проверки количества найденных n-грамм
 * @param seq список последовательностей
 * @return    количество найденных n-грамм
 */
const size_t anyks::Alm::grams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq список слов последовательности
		 * @return    результат проверки
		 */
		auto checkFn = [this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			bool result = false;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						result = (i == (seq.size() - 1));
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
			}
			// Выводим результат
			return result;
		};
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && checkFn(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * bigrams Метод проверки количества найденных в тексте биграмм
 * @param text текст для расчёта
 * @return     количество найденных биграмм
 */
const size_t anyks::Alm::bigrams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty() && !this->arpa.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->bigrams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * bigrams Метод проверки количества найденных биграмм
 * @param seq список последовательностей
 * @return    количество найденных биграмм
 */
const size_t anyks::Alm::bigrams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty() && (seq.size() > 1) && !this->arpa.empty()){
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq список слов последовательности
		 * @return    результат проверки
		 */
		auto checkFn = [this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			bool result = false;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						result = (i == (seq.size() - 1));
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
			}
			// Выводим результат
			return result;
		};
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = 2;
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && checkFn(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * trigrams Метод проверки количества найденных в тексте триграмм
 * @param text текст для расчёта
 * @return     количество найденных триграмм
 */
const size_t anyks::Alm::trigrams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty() && !this->arpa.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->trigrams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * trigrams Метод проверки количества найденных последовательных триграмм
 * @param seq список последовательностей
 * @return    количество найденных триграмм
 */
const size_t anyks::Alm::trigrams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty() && (seq.size() > 2) && !this->arpa.empty()){
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq список слов последовательности
		 * @return    результат проверки
		 */
		auto checkFn = [this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			bool result = false;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						result = (i == (seq.size() - 1));
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
			}
			// Выводим результат
			return result;
		};
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = 3;
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && checkFn(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
 * @param idw идентификатор пользовательского токена
 * @return    пользовательский токен соответствующий идентификатору
 */
const string anyks::Alm::getUserTokenWord(const size_t idw) const noexcept {
	// Результат работы функции
	string result = "";
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)){
		// Ищем идентификатор токена
		auto it = this->utokens.find(idw);
		// Если такой идентификатор существует
		if(it != this->utokens.end()){
			// Выводим слово соответствующее идентификатору
			result = it->second.name.real();
			// Выполняем удаление экранирования
			this->clearShielding(result, "<", ">");
		}
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenId Метод получения идентификатора пользовательского токена
 * @param name слово для которого нужно получить идентификатор
 * @return     идентификатор пользовательского токена соответствующий слову
 */
const size_t anyks::Alm::getUserTokenId(const string & name) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!name.empty()){
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название токена
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
			// Если такой идентификатор существует, выводим его
			result = (this->utokens.count(idw) > 0 ? idw : result);
		}
	}
	// Выводим результат
	return result;
}
/**
 * Alm Конструктор
 * @param alphabet объект алфавита
 */
anyks::Alm::Alm(const alphabet_t * alphabet) noexcept {
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
}
/**
 * Alm Конструктор
 * @param tokenizer объект токенизатора
 */
anyks::Alm::Alm(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * Alm Конструктор
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 */
anyks::Alm::Alm(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * ~Alm Деструктор
 */
anyks::Alm::~Alm() noexcept {
	// Очищаем языковую модель
	this->clear();
	// Очищаем выделенную память под python
	if(!notCleanPython && (this->python != nullptr)) delete this->python;
}