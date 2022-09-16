import os
import time
import mysql.connector
import requests

SOLAR_ENDPOINT = os.getenv('SOLAR_ENDPOINT')
FETCH_INTERVAL = float(os.getenv('FETCH_INTERVAL'))

while True:
    try:
        db = mysql.connector.connect(
            host=os.getenv('DB_HOST'),
            user=os.getenv('DB_USER'),
            password=os.getenv('DB_PASSWORD'),
        )
        break
    except Exception:
        print('Could not connect to mysql server. Attempting again in a few seconds..')
        time.sleep(5)


print('Connected to the database.')

db.database = os.getenv('DB_DATABASE')


def fetch_current_solar_data():
    try:
        res = requests.request(url=SOLAR_ENDPOINT, method='GET')
        json = res.json()
        if json['aliveMs'] == 0:
            return None

        wh = json['pulseCount']
        watts = json['calculatedWattage']

        if watts < 1.0:
            watts = 0.0

        return {
            'wh': wh,
            'watts': watts,
        }
    except Exception:
        return None


def fetch_last_db_data():
    cursor = db.cursor()

    cursor.execute('SELECT wh_since_reset, wh_total FROM rb_solar WHERE 1 ORDER BY timestamp DESC LIMIT 1')
    res = cursor.fetchall()

    if len(res) == 0:
        return {
            'wh_since_reset': 0,
            'wh_total': 0,
        }
    else:
        return {
            'wh_since_reset': res[0][0],
            'wh_total': res[0][1],
        }


def store_in_db(wh, watts=0):
    last_data = fetch_last_db_data()

    if last_data['wh_since_reset'] > wh:
        # The last stored Wh are greater than the current 'total' Wh (which are the wH
        # since the device got reset). This means that the device must be reset.
        # This isn't totally foolproof, for example the device could have recorded more
        # wH than before it was reset when we fetch it, but it's good enough (hopefully).
        wh_since_last = wh
    else:
        wh_since_last = wh - last_data['wh_since_reset']

    timestamp = int(time.time())
    wh_total = last_data['wh_total'] + wh_since_last

    cursor = db.cursor()

    cursor.execute(f'INSERT INTO rb_solar(timestamp, wh_since_reset, wh_total, watts)'
                   f'VALUES ({timestamp}, {wh}, {wh_total}, {watts})')

    db.commit()


def fetch_and_store():
    try:
        data = fetch_current_solar_data()
        print(f'Fetched data. {data=}')

        if data is not None:
            store_in_db(wh=data['wh'], watts=data['watts'])
            print('Stored data.')
    except Exception as e:
        print('Could not fetch/store data.', e)


while True:
    fetch_and_store()
    time.sleep(FETCH_INTERVAL)
