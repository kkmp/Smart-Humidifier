import { Fragment, useState, useEffect } from "react"
import axios from 'axios'
import Plot from 'react-plotly.js'
import LoadingComponent from '../LoadingComponent'
import ReactDatePicker from "react-datepicker"
import 'react-datepicker/dist/react-datepicker.css'
import { Button } from "react-bootstrap"


const HistoryComponent = ({ espIp }) => {
    const [dataHook, setDataHook] = useState([])
    var data = []
    const [loading, setLoading] = useState(false)
    const [showHumidity, setShowHumidity] = useState(false)
    const [selectedOption, setSelectedOption] = useState('Humidity')
    const [fileNames, setFileNames] = useState([])
    const [selectedDate, setSelectedDate] = useState(new Date())


    useEffect(() => {
        const fetchFileNames = async () => {
            try {
                const response = await axios.get(`http://${espIp}/listFiles`)
                if (response.data.length > 0) {
                    let preparedData = response.data.map((item) => new Date(item.File.replace("/", "").replace(".csv", "")))
                    setFileNames(preparedData)
                }
            } catch (error) {
                console.error('An error occurred while downloading data:', error)
            }
        }
        fetchFileNames()
    }, []);

    const fetchData = async (pageNumber) => {
        const year = selectedDate.getFullYear()
        const month = String(selectedDate.getMonth() + 1).padStart(2, '0')
        const day = String(selectedDate.getDate()).padStart(2, '0')

        const formattedDate = `${year}-${month}-${day}`

        try {
            const response = await axios.get(`http://${espIp}/data?page=${pageNumber}&date=${formattedDate}`)
            if (response.data.length === 0) {
                return false
            }
            data.push(...response.data)
            return true
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    };

    const fillWithNulls = () => {
        let newData = []

        let i = 0
        for (let hour = 0; hour <= 23; hour++) {
            for (let minute = 0; minute <= 59; minute++) {
                const formattedHour = hour.toString().padStart(2, '0')
                const formattedMinute = minute.toString().padStart(2, '0')
                const time = `${formattedHour}:${formattedMinute}`;
                if (i < data.length && time === data[i].Time.slice(0, 5)) {
                    newData.push(data[i])
                    i++
                } else {
                    newData.push({ "Time": time, "Humidity": null, "Temperature": null, "Pressure": null })
                }
            }
        }

        return newData
    }

    const handleClick = async () => {
        setShowHumidity(false)
        setLoading(true)
        data = []

        let page = 1
        let status = 1
        while (status) {
            console.log(`Page: ${page}`)
            status = await fetchData(page)
            page++
        }

        setDataHook(fillWithNulls())

        setLoading(false)
        setShowHumidity(true)
    };

    const handleOptionClick = (option) => {
        setSelectedOption(option);
    };

    const handleDateChange = (date) => {
        setSelectedDate(date);
    };

    return (
        <Fragment>
            <div className="workspace-centered">
                <div className="box-row">
                    <div className="box-parameters">
                        <div className="box-col">
                            <div className="box-text">
                                History of measurements
                            </div>
                        </div>
                    </div>
                </div>
                <div className="row-many-elements">
                    <div className="fileNames-box">
                        <ReactDatePicker
                            selected={selectedDate}
                            onChange={handleDateChange}
                            highlightDates={fileNames}
                        />
                    </div>
                    <div className="btn-group" role="group" aria-label="Basic example">
                        <button type="button" className={`btn btn-primary ${selectedOption === 'Humidity' ? 'active' : ''}`} onClick={() => handleOptionClick('Humidity')}>Humidity plot</button>
                        <button type="button" className={`btn btn-primary ${selectedOption === 'Temperature' ? 'active' : ''}`} onClick={() => handleOptionClick('Temperature')}>Temperature plot</button>
                        <button type="button" className={`btn btn-primary ${selectedOption === 'Pressure' ? 'active' : ''}`} onClick={() => handleOptionClick('Pressure')}>Pressure plot</button>
                    </div>
                    <Button onClick={handleClick}>Load data</Button>
                </div>
                <div className="plot">
                    {showHumidity ?
                        <Plot
                            data={[
                                {
                                    x: dataHook.map((item) => item.Time),
                                    y: (() => {
                                        switch (selectedOption) {
                                            case 'Humidity':
                                                return dataHook.map((item) => item.Humidity);
                                            case 'Temperature':
                                                return dataHook.map((item) => item.Temperature);
                                            default:
                                                return dataHook.map((item) => item.Pressure);
                                        }
                                    })(),
                                    type: 'scatter',
                                },
                            ]}
                            layout={{ title: `${selectedOption} plot`, width: 1200, height: 550 }}
                        /> :
                        loading ?
                            <div className="row-button">
                                Loading data. Please wait...
                                <LoadingComponent />
                            </div>
                            : null}
                </div>
            </div>
        </Fragment >
    );
};

export default HistoryComponent;