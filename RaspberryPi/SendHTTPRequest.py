import requests

URL = "https://ims_api.supppee.workers.dev/api/example"

r = requests.get(url = URL)

data = r.json()

print(data)
