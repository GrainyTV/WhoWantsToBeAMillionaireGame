import itertools
import math
import requests
import time
import tomli_w

from dataclasses import dataclass, asdict
from urllib.parse import unquote

WEBSITE = "https://opentdb.com"

@dataclass
class TriviaEntry:
    difficulty: str
    question: str
    correctAnswer: str
    incorrectAnswers: list[str]

def scrapeItems(url: str):
    entriesJson = requests.get(url).json()
    assert int(entriesJson["response_code"]) == 0
    time.sleep(5.1)

    return [
        TriviaEntry(
            difficulty=unquote(entry["difficulty"]),
            question=unquote(entry["question"]),
            correctAnswer=unquote(entry["correct_answer"]),
            incorrectAnswers=[unquote(ans) for ans in entry["incorrect_answers"]]
        )
        for entry in entriesJson["results"]
        if entry["type"] == "multiple"
    ]

def requestAccessToken() -> str:
    accessTokenJson = requests.get(WEBSITE + "/api_token.php?command=request").json()
    return accessTokenJson["token"]

def fetchEntries(id: int, categoryName: str, token: str):
    triviaCountJson = requests.get(WEBSITE + f"/api_count.php?category={id}").json()
    count = triviaCountJson["category_question_count"]["total_question_count"]
    batchSize = 50
    numberOfBatches = math.ceil(count / batchSize)
    
    results = list(itertools.chain.from_iterable([
        scrapeItems(WEBSITE + f"/api.php?encode=url3986&token={token}&category={id}&amount={batchSize if i < numberOfBatches - 1 else count % batchSize}")
        for i in range(numberOfBatches)
    ]))

    with open(f"{categoryName}.toml", "wb") as output:
        entries = [asdict(result) for result in results]
        tomli_w.dump({ "entry": entries }, output, indent=2)

def main():
    categories = {
        9: "general",
        10: "books",
        11: "films",
        12: "music",
        13: "theatre",
        14: "television",
        15: "videogames",
        16: "boardgames",
        17: "nature",
        18: "computers",
        19: "maths",
        20: "mythology",
        21: "sports",
        22: "geography",
        23: "history",
        24: "politics",
        25: "art",
        26: "celebrities",
        27: "animals",
        28: "vehicles",
        29: "comics",
        30: "gadgets",
        31: "otaku",
        32: "cartoon"
    }

    token = requestAccessToken()
    [fetchEntries(k, v, token) for k, v in categories.items()]

if __name__ == '__main__':
    main()
