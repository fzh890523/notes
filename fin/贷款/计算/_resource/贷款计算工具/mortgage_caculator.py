from typing import List

class MonthlyPayment(object):
    def __init__(self, m_corpus=0.0, m_interest=0.0):
        self.m_corpus = m_corpus
        self.m_interest = m_interest


class LoanStatus(object):
    def __init__(self, rest_corpus: float, payed_interest: float, used_corpus: float=0.0):
        self.rest_corpus = rest_corpus
        self.payed_interest = payed_interest
        self.used_corpus = used_corpus


class MonthlyAction(object):
    def __init__(self, month: int, mp: MonthlyPayment, ls: LoanStatus):
        self.mp = mp
        self.ls = ls


class LoanResult(object):
    def __init__(self, m_actions: List[MonthlyAction]):
        self.m_actions = m_actions


def equal_corpus_interest(loan: float, months: int, rate: float) -> LoanResult:
    m_rate = rate / 12
    m_avg = loan * m_rate * pow((1+m_rate), months) / (pow((1+m_rate), months)-1)
    
    rest_corpus = loan
    m_actions = []
    payed_interest = 0.0
    used_corpus = 0.0

    for i in range(months):
        m_interest = rest_corpus * m_rate
        m_corpus = m_avg - m_interest
        used_corpus += rest_corpus
        rest_corpus -= m_corpus
        payed_interest += m_interest

        mp = MonthlyPayment(m_corpus, m_interest)
        ls = LoanStatus(rest_corpus, payed_interest, used_corpus)

        m_actions.append(MonthlyAction(i+1, mp, ls))

    return LoanResult(m_actions)


def equal_corpus(loan: float, months: int, rate: float) -> List[MonthlyAction]:
    m_corpus = loan / months

    m_rate = rate / 12
    rest_corpus = loan
    m_actions = []
    payed_interest = 0.0
    used_corpus = 0.0

    for i in range(months):
        m_interest = rest_corpus * m_rate
        used_corpus += rest_corpus
        rest_corpus -= m_corpus
        payed_interest += m_interest

        mp = MonthlyPayment(m_corpus, m_interest)
        ls = LoanStatus(rest_corpus, payed_interest, used_corpus)

        m_actions.append(MonthlyAction(i+1, mp, ls))

    return LoanResult(m_actions)

# jsonpickle.dumps(mortgage_caculator.equal_corpus(200000, 36, 4.7 / 100).m_actions[:12], indent="  ", unpicklable=False)
