<div align="center">

<a href="https://github.com/pasquale90/interactive-audio-visualizer"><img src="files/imgs/interactive-audio-visualizer.svg" width="650"></a>

<a href="https://github.com/pasquale90/interactive-audio-visualizer"><img src="files/imgs/iav.gif" width="650"></a>

### *Bringing Sound to Life with Interactive Real-Time Audio-Visual Experience*

[![CI Status](https://github.com/pasquale90/interactive-audio-visualizer/actions/workflows/ci-workflow.yml/badge.svg)](https://github.com/pasquale90/interactive-audio-visualizer/actions/workflows/ci-workflow.yml)
[![Licence](https://img.shields.io/badge/licence%20-%20MIT-blue)](https://github.com/pasquale90/interactive-audio-visualizer?tab=MIT-1-ov-file)
[![Version](https://img.shields.io/badge/version-2.0-green)](https://github.com/pasquale90/interactive-audio-visualizer)
[![Demo](https://img.shields.io/badge/demo-youtube-red)](https://www.youtube.com/watch?v=y8mJdEyd31Y&ab_channel=PasxalisMelissas)
[![Gitter](https://img.shields.io/badge/chat-Gitter-yellow)](https://matrix.to/#/#interactive-audio-visualizer:gitter.im)

<div align="left">

## Contents 

- <a href="#description">Description</a>
- <a href="#documentation">Documentation</a> 
- <a href="#build">Build</a> 
- <a href="#docker">Docker</a> 
- <a href="#usage">Usage</a> 
- <a href="#version-history">Version history</a>
- <a href="#future-work">Future Work</a> 
- <a href="#feedback">Feedback</a>
- <a href="#related">Related</a>


## Description
This project develops an innovative interactive tool designed to offer a real-time audiovisual experience for users. The envisioned objective of IAV is to provide individuals with limited mobility or minimal musical background an accessible way to engage with positive musical stimuli, using simple actions like hand movements, to inspire and motivate them to participate in creative expression.

## Documentation

You can access the full documentation for the project in PDF format by clicking [here](files/docs/documentation.pdf). 

If you'd like to generate the documentation in different formats using **Doxygen**, run the following command in your terminal:
```
cd interactive-audio-visualizer
doxygen files/docs/Doxyfile
```
You may then find the generated documentation in the `files/docs/documentation/` folder.
For example, you can open the `files/docs/documentation/html/index.html` file in your web browser to view the webpage-like documentation.

A UML class diagram that illustrates the structure of all the classes in the project is available for reference. You can view it by clicking the following link: [UML Class Diagram Image](files/design/all_classes.png) or check it online using the [draw.io online viewer](https://viewer.diagrams.net/?tags=%7B%7D&lightbox=1&highlight=0000ff&edit=_blank&layers=1&nav=1&title=uml.drawio#R%3Cmxfile%3E%3Cdiagram%20name%3D%22Page-1%22%20id%3D%224lq8rmeidR7subM9_Ofi%22%3E7Z1bc9tGssc%2Fjap8XCUV7pdHWY6yPmWvE0nr7D6xYGBI4ZgEGACkrTzks5%2FBlQCmSZEyZ9DaaieVmOBF4vQPPd3%2F6em5MG9WP37NgvXjpzRiywtDi35cmO8vDEO3TIv%2Fr7zyVF9xPaO%2BsMjiqHnR7sJ9%2FBdrLmrN1U0csXzwwiJNl0W8Hl4M0yRhYTG4FmRZ%2Bn34snm6HP7UdbBgwoX7MFiKV%2F%2BIo%2BKxvurZ2u76P1i8eGx%2Fsq41z6yC9sXNhfwxiNLvvUvmLxfmTZamRf231Y8btiwHrx2X%2Bn23e57tfrGMJcUxb%2Fi8%2BvD06eP156%2Bf8j8e1r%2F85%2B598O2y%2BZRtsNw0X%2FiOLeI0%2BTz%2FkBQsY3nR%2FPLFUzsi%2Bfd4tQwS%2FujdPE2K%2B%2BYZnT8OlvEi4X8PWflmfmHLsiLmg3ndPFGka341fIyX0cfgKd2Uv3heBOG39tG7xzSL%2F%2BIfGyybz%2BRPZ0XDhWENXnFfvpNf1vhV%2Fqvy1%2FzWjoY%2BuvQp%2BDF44ccgL5oLYbpcBus8%2Flp9jfLKKsgWcfIuLYp01VwSB7sdOf4N2Y%2FepWbwf2XpihXZE39J86yv2%2FVbmjvB1DynvvB9B5bRgvXYh8ps7pmggXnRffjO3vwvjclPML8hmP%2FCeFdb79%2F8%2BoV5Xdk%2BKv9iXgd8POLwwnCCVWnGZWmvmI9He2FRdCPV44UPT1HZMUu%2FsZt0mXIw3idpDVC8XI4utQwt2bzYS1C%2BDsI4WXysXvPe2l25a0atvJTy986X1S33GEcRS0rrp0VQBDtTr1P%2BBapRtd%2Fxf%2Fng32hX9oXNf%2FEb%2FljfPeb%2Fli%2FPips04d8liCsQGOfoe3mXQIgcvOOe56bhxLCOo6R93dkhMfdD8h%2BCBAckljcxJBYEyTZdblbsHwQJDkhcY2JI7P2Q%2FEGQ4IDEPzIokQaJI5h0GVemqk3aRuH6i%2By54pZZsp0BH0r7vr%2FUBSObopFNwKDL4Ctb%2FpbmccGDZn4tq187MvRzthzCGiePLItl2lhvc5JnI09ZgacreILrTRSnfIDm8QJVymFe2aZnOXrz39ecgVhOm3y3KYituQIIpm0AINj2cBxsSWB40BQRlGy8Z9s4ZOI0wW8efpPSPHDYR7hHw9KmHQL4R4EibVrwITC%2BbuZzllW3KoUPKLCxXFzYtDrikJtks7p5DJKELfPjwNkkObcoi8ofRwxJZsjVwblmOoZElZQz9OcmSIr4r6CK%2FFqIKjgIh7Pi4MOhx3Q4gKppzj3Ckt0FBU1FSLjRdR8ZOCaltiemtqcb3TJfZHRpma4uKqM3wYplAcJc9zUnt6bteqPkVgdEcc8HbC9tfU0HFc8wWN2xnGRxSYqndTQ6B1bYIE7kTQzOfkxIGMeCCbTGphYTUTXlmESkiynFAFpFU4sBqJHOMx5VHJ9%2BROmmHHZyGVJZgRbT1LLiU8px6mrayVYGl9PAQFNWnGmIKuf7OF8vgyeMWcaVM%2FjjveKsw9BdbZR1GFApjgPAoOvDgXBlwQHKlxHng7IQaXNFd0eekIUI98VRHBmylmINUOdsuKG0BAs3lg%2B6kemwAetF5%2BsjV9ooMFUBjWshg8aiOPXEOPV0o%2Fsv8xTyolZRHf1w%2FQVhxPpfVAOme%2B5oGwpUA6Z7EAeqasAMUA5dBT9uM%2FbnhiXh0wWttx%2FvJ%2ByjGTm69AvkQ5qIYYC65ypOiAclPDxf06WYB1AAzdL4LojiTU4wyITh%2BeIsxTCAhaF8SMJv3ALXywWtjKhC4%2FlCLbVomGDtJzf%2BYsHDRMJCERZH1GEp5kKnZPPUZPN0qz9fiAVnGbKyzbbIp%2BcN7llR8NHP34sK07TppmsM%2FnnF%2BaZvOYN0U3csMd102i4hg9ufRxqDUZCVbpqiYHkZfd3ND38u44KZF10fD5oanrvDTsk0x6gfx4a0qUGs1uQs%2FFb2cqF4QREUPN0cIGFOzYRN4cKJ4cLpRh87e%2BM4o3uybA5KkLt44U1YDhzkDbRu6Yr%2FPfo6W7e%2Bg3%2BGVjqSvH59VFotyFl56X8651L%2Fd5vyX478yrkR81%2BEmDy%2FAsqaAGP9MHWHF0GjAhpdG4coU1MDip89amBaCBu12BguMmxAmfTvHjcEhhIw7JcFOvIad4EiabpmWVBUpjxxGtrnfgikM4Pk2shAAstM%2ByCdODMRSYpI8pEFxpao1HKSlmkQtUgIk1WXhm2SNItYxqLZKlgPKwuFVO1GuEJFh1LVPkHumxo0sFQ1D7asA03I888J2EAryJsfOUb7a5ouicWzszhWGSdnEegTgLES0hz8MS5e79KU4bfScteRWxcpsNo1iQEFnjUcCFlQgP0Dgk3E3dK88xCHWieSo4DvslOWqMbIH4eINCb2NQsYMHGwxwhB8ZNQWM4IiqmZgDsDxPl6AMXhPaFExU9S4fjIqACl4jjYDqDYv%2BWCgPhJIHwDGRCiCHzZph2USr8msnTdxoWWLVZHUZHE2a1uekOr68dZXVaVhA1KvvVs0ikn7eRCK9eKXcQ4SD0SFnkuQpR1LxtWiAglRIwD1MmJAPVXaO2RnMhEyIxD2MmRAQ%2BaiurElvyIGklsHHxODgUoky5Y8YGPTJCETABD9CeEyfmV05dFq%2FIwEZXTy2yTXFddaOLi6eaRhd8EVGj9TQocjg6toEzHhqigfokjlj6UG4nL3aKIFuGsK9O1L17vwpuuOcMeJIYFbPKwWlsP7K9p1ZeXBIEomF5%2BTTdJxG%2FCd%2BmPziWE21rruitPnCbXcNA12Ce38G%2F4Po4HaUdVAlJpfUMTCKpAsDwEIDhifexlON91laBIUj4IroEBBFHsvCza6GDkD34rsuGCSftEF07Q8ohMYHwHAzCGYFhaEjm3pXVDO97UstZBHFDH7OcPpEwpue91A0XIAGqUccJvLW6yv1jLRCtut5PDp6AghVs5MjaK4AJUMKvw4vPX%2F%2BPZxXOwlCNqaHdswb335%2FmHMlFh%2BUGcSNeSgpOLIvQQlc66IFQw%2BJQ6lnHlGv0%2Fr1jSMm1zePqc4QPHRZk6AIFl6MNxkCVxOqLEeVl3YC%2FPQl4w8gwneIbuDjth99MY96PwkIcDIHaGlZMYyBuDOnISOeTxYdnI%2BIAU0LIQdKR1VGnOTbAuNhkjKs5NheOBk8NkVLiAHFodPSdwUQWohMN5cfDhWGE6HABRtMLhIV0slkeeRVjHG6SIygRH1y1k5BgiOUUFze3AnVATdxk0mC4yGkzByKSVn93qjvYiq8uSzV1QJ60zDhLM1TgCdPElKIQ2THTV4I2WRVq5YlrQhZ%2FgXviGFpESwkWt4oUu5gS3yf9NM45KKNCFnuAueXAT0rPTDuRyCKAzA%2FTCKFYeQGBr1T5AJ8xERJACgjwTF0Ee2IM1rDX1N0eWh9CqnQxWTM1Bxgq4577SXGesXOtmEW1hUoOG4SNDQ5RU622QsyAsNsFyNl%2BLnVNJX5WBhm0gQwOsUC3RCDdZ%2BfVnlQc5dq6h1EgKNa6NjBrgNKxdGesspLxZIRw%2BMqXWE5Xah3iFbKuseWVb%2FX9e81GKum4Pz1I0LeAobkvzAQjKU5v7A%2BHIggLYYt94jHsWpklEp3Of4CW8k09dBYA%2FDhBZlcgeUHS65APxr3UUFKx0GB0QXY1I%2BJilSdq2E2RB9DQLl2l5p5ZXCv6eWWULwufc%2BFjuEB%2Bgjl0tPUCNasZWfETKVXwOwidu1TgnzyIfjfEMMjkavmBjqhg5t9F9%2B0VGl1Uw4oM6aRV1UhaixAuUxyGicgM%2BKIcuWHFT6xolHG%2BqCYGUc8WoGMiCCX%2BfPHrXjygER8LHs14ALkeXYgspqNhwejodKqBcGufVVHPLWckfgWWWISnkVaSg4trIUAGLWNfBJmcUmCjkwveRcQEWsuasqKngI0NgKFHExpLY5GCANauVak2%2BQg0SY5lrciREkfQhixcLZIsp1pU%2B%2BHPxitdSLH20lqJZ4lqK0XbyGVAgjoMkKgDxszy8qbc7v3d2ExUMHusw%2FJd0JIUs%2Fiwp0vwFsDV%2FlUbUwUMuBpY3wuBYjyGLA10DduOX62PvN1lQKs33LLygVRGJSLgGLs%2Bga1CX0roso%2BEALtIgEn6SBN%2FBRoIhGJkWyM5t9bL96EvMLmuFTNdA5bLJJii%2FVOIKyi6kyHwBUMg5KyPGuk0LcaGGCxtdtACKlHFedajlVvglKYc4esNHnzYfTQCMiy6oAMXLjOWMxEtFWoT2sohDIhOievlHsGXzNFsJ5p%2B2FlxzBn9es37pt7X%2BXeGvL5b6Gq2E2AfBtMcDIQ0MqMMoHZ3yE%2B5id6%2BdUhE%2Bxv4oTKTtG9E1SL0MdocqNcEGkXBmEiwHvO%2BnA0EH1MtVnBAIkkFwfGwgAKJlxoJone6K%2Bg92EC1n4VlBPUQlg%2BMb2MAxRHC%2B89jzoRpZAZ0tC8uOLAN0GidD5EglR9dtbOiIQunl9ywuGDkdZOiYHjZ0LMHMtLJyfrs7%2BovsLm1pRQfF0lbqIPlLkTvAF72CmmiwDeJlObi3aXbHY1luj2c2DLTzCSFzbmTwxa1gI9IFKz7FyafgR7fe8pacihqRDF94CnYljfNfVuviiXYeTaOkogtEwc6jcX67WS6JkUkYeWHQKo8RA9wzX6prtKo%2FFSSehQ0ScBd9GZBSzDoNI6bmYmNE1Fo5I5Vk1nVIJ48yES2mho0WUV69X7OQm3iRBbjqQowr6%2BL1VoIYvt3cmN2G17ZNTn8%2Fo6WJ1rdt70paVZABlJxS8cdP%2BYXunjrlzGGgQyTEQuvdJYAgyqaX83kx6y30H1ygi9JNtZhHyyxSUbEsBKgA3UNLVEov3hWFzIvvszBdrZesKhkiEs5NguMgIAFoG%2FoYrFZ8zGff4yTiw0yr%2B2iI8TwExADFpqvgx6dgkcTFprc3mqrK5ECg6xoCCqDa0nJNhShQRIGJIOQ0gcLSZLO63kRxeh%2BUoQM1C5aMgY0gnDSBslKOwe3tw2%2F1GBIDUhlwEQSSpqhaXq6XQa%2FIvMwn6itEwLkJ8BEEhiZQ6EnF5fhgMQwE8aMJqJYZH%2FF3m%2Fm810OFcs7pcbEwBJqAtklF5BhpcTDEo45gWaobP7%2BpvaPDTmml4iZY39lbAKVqcUX3vY%2FhvgdrOfs0tPUTgyVyOn5TMSymjiFhBWs6%2B7DsgYRoUUyLiSC5tUSlk9PyN8006nFwEKSvFljHma5ZFhSV%2FU6dafb6GuLn3Px4CPJZS1RLR%2FycOvkQQKoAsjQEoa4FiK3cLuFmGRTsn7e3DzQdKaLBQBDLWqKaWm9znI1WYwUqaEVOTo0fhpAV3EbfQNGtzRIRaohwMUSt%2Bw%2BNL9Xxpu6PjsxSR4WPIRYFNdRy5fZdUISPb05fg%2BuHn%2FyXEncr7blMm5iUUGfrGAJYUKulXW4oADExxLSiPvslzjfVVJUJRp9yX5t%2B5bn2xevd2aY7bQFfV1vamrpnfs8xRPN7nlt9eUkM2EB96TuKVE%2B4%2Bbu76IQWLTuLPmd%2BeXe%2FDZSU%2FkqWl2p5y0RheVEavfx4R6aXanrHRmF6QNQky8u1fH8Cn9DyQDnoA830ck2vaz4K2wO1nWGwehcnQfb0Kci%2FdRiE21qI%2BEQ7ymTgYOKI%2FACRkuPAsqDj4KZ5SAicGwEbRwgIbEqvEbjN%2BP%2FIH6iBwcURFD5zuFErFlKfGxVM%2BDjCRWD3ebJZ1Suav7Es5j%2Bnt4uIwkc5QhGK8NEBNMJ8zUKxFQFtJpueGQtFjOkA6mI%2BLKATi%2BoIhnPD4KCINtuf1oehaM7lrY9tvqD1RrkgeCgiTQeQH%2FlAJPk6zdksC4o4ndHpibJhKBvyYoABUCTHMDwRDLJhMHDEmIBEWWTxYtFLMh7axwTBuSGwcQSNgDC5WUdBwaoK6491Xco415hvkrDa2D3INup6yp1UseTjdmG%2Ba7WMOs642fPwji34B36efyjrWVg%2Brqyrsl1gx0hW%2FwzKcWTj6uIIawERdRtHLH0oY9seql8GFwmHc%2BPg4whuARl1W9XTkaausMRGxxHcAvrp9%2BZ8xY6EP7oLhMG5MTBRhLWu2NWb%2BuSc39jOCeGrtE45LrhjfVdRTftFFd36Do5bXxQ9hzi0yUa%2F5p4a5aiGxUeR%2BbqiMDqEBYaEaFFLi62jSDxdcDP63zTVKOfBRJF5uuA%2BdKhXznFzzT5nQ%2Fycmx8bRbLqgse%2B9%2Fk5cfohgFQB5OKIdQEldBZmLCjYLAxWZYk5TUmK9hlpOAJaQAyd5ax4SOvS8vrEZmJCDRMGjrAV0ERLJmbvfp2tgoTPGMsnao2ijgoLRfDqAdWlFRW3PSqOWZglbNRg46CIWT2gwHSWP6bfq8U2IdqgukI5MHgo4k9P1FprGGZFzKMNCjYUIuHqKAJQD1RUv2ZpEIV8GCgdUUSDiSL09IBa0ygL2gV4okERDTaOkBMoNuU0fL9f8WAzjLNwySjiRESNiyPiBKpTK2rohALlRPg4wk6wryfPXaty5Ur%2F%2FldVvpy9eS3VygSwmp5AOHrAeWCL0F7J%2FT03UbCk%2FEklGjiaxHmAWFuD8dDs3PyURuxN6YoIDDVg4Ggh54t67X1Z0jo2%2BJT9YvkddPF6u8Vamt8EFt2Gzfbk557Z7faDB2a3eKgszfKA5BqUswTULKaaPqhjzMl%2Bobu9jt%2FbbbYmf54OaWgAAuz6Mch3u15o%2F64c6%2B9u%2BAmtD2zrX2dsO8%2FYnx0A1CJIhvldHYH5oZPmgyifrVk2y6tjkcgNSOag3AI5OQeArFqnDMN6wJ%2FYvj3sIUX7rdXApZd9yCani46cP3kr3QtMbR0dTcrLM%2BAj57mxSV9XdMPbGMIKUJzMWfEpTVph8qE04d5jr96%2BJV7U8OJiCD9ExbLm5b5cFiFicBHjTx9SGBp45DwnZhev%2Ftz63RHhKvGmRiRDoJIZmiig1u4IkXRuX7m6ZXh2%2B9%2FXfOya6esjId02fQECoz0apQ%2BBrw3HQdZyiqGJyin3QaVytmFJuOt%2F2DmggA9RHA7dT6WtURJ8vAfZ3YrHexDxzjiKJMORhg5Y4rpNl5sV0JIb4oZacqsghydT2MixBDOTlnJ2u7suOIc8Z3bdlGZ2UaCtq3yKjAH916cMQ4wr81Wf%2BGoZbVVGt0qrifYH48%2FybGBT2omvhgaUsdIq%2FrknCftoYLoEZWf0ZzMUedEodC7UMmZ1CWmFxv%2Fx23BWX5vVhaXEwplZsCwULEDHQtV2Twb9TBtfET4GGfEggwfHQcEDUBG6Cr6xWZ5ukmgkYAlSWFUjWCUib%2FcKYs1LAYl2p8%2BKbybczoyb52HATQf6AvDIcb0p3m3mc5blF6OaEu0tH0GjGktC4sxI6LqGggmgArVmYlaOyw6JKkgpL80IDLlgmCjiVl1UUS%2FzONkFKWCJOpn%2Fp81vowhVdehcqTRi2zjcG6e%2BJRzOj4OLIlLVoZOl0p43AFfdyPw%2FbX4fR%2BRoC7YldVuCYHVCSCirVtDQwdalPVWbSgZV6Zc47n2wdrTPw0Dablc%2BqKeHalxsHAklWGPax2UPJsSLYl5cHHkmWGP6N8036oHwUWSeBlhCCrXeP3K%2B2etviKAzE2TqKJJVQ9Q0RwSdOgURQsoQMlEEve0nDxGq12r5oCQsLHYF7KtN%2BeWEHRDd89xfRVUuOtsGWVxZRHhx3YCKgmbVuDkogmYDrDsNl2nOKAZSyoOHIig2ALm1WoDLHzdFlH5P3jQ1AW%2FLH58tiA41VUQajggZPJyKNln9NxJn4IioQU2YE7dZN4WLXVIGlbZq4ZxclKpCRxzxM1D1mleRTJVKvakms2ReHiyRVwUlzd%2FrYPjw5EZtj6SA4%2BKIhOEWBRU6dZWaEAoTD1J48HFEwqA8XPdAelNZvms2QDOMEjBsHUUQbIoycTW1%2FCPIou%2F8a9dh74dkngoMTLsZXB%2F8uXi9G7Jaob7lou3m32PCcgAmdN0cD4IsRkAhOOSEzOIKjPGO3nUQZ8MMqXuK32X8juZ3oHCF9vqe4G%2B62%2FaUXeIQLc9R1ibJEqgCteFks%2FrKss%2Fzm8cgSdhyVzi9SXJuPBZRpCIJEFsbAWJNTggo59Z9O2dZeYRvFiQLoKOA6H%2FG9NyML5D3kQqXi877UKOBk0sxTze7ro2dypGxjbROA6Yo%2BtbnPiMMcf97%2Bh3ZbQjZzjW%2B2PLKbA8tmqTbkQlKs%2FVGjd8C%2FjsKk0wbt9JccdhpvKAC89muNCAr8rrSmGDtLr%2Bb0uWm9Mi5SMeWhWWRDJwEifFJHZQAkQjFJVJZO6IDkmrWPApMTg5MTrb78x2QQLNLjEtEgXZ3Vh%2BquMS8es2RCL9zh6GIYbXaeM%2F4jmUA97xvX0nLRyygqUBYBab97kd1qEptj06fGU4%2B1Kzi%2FDgupLVltYCuAl0DzruB9kGamAwILBcBBKJOehkH275b%2BHD9hXyCAhxcHQEOe460mtGZVtLtvwsAJrQ%2FUNPaOxdXzERPKlms81A6zGgKusrDjKbHCzgqa9S4mU5Kk2R%2FC0PISYdZnS4%2FnG5q5%2BjAUlqDCmt%2FQ4JKdKC9MopuexdDVAG0VF1kQbQJlk%2BzeRA1RYK0sU41HD6GmGDf%2BVWVs%2Fi5PTJHB5wEnBrZC4PuZQNiKB%2BGJF%2BW5crtFpk7tuCUfZ5%2FKIVulg%2FcU13P%2FIzPqjcLE0LnRgiDamaDNavFJktmH1bBgs34gKSz%2B7JBdAdUzUMJzv4rBB1S6DBoc7Yo1dYtL1i2RbaKZ1y5F693Fc9y2%2FNpuqY7BrCVonUCfetbPPVypQEAiLN0hMmZnUN3l51yhIl7HB2GtAV%2BG9BtoyzmX4IOrVBLg2UjoAGQWWsacoGE%2F73%2FGOd0oI0UFhwPAQvAuVf8u3KnUBAOanHwdQQ4QHv%2BmwBS2x0iERbLWX2ZDruSA4OuWwhoAITZPF6sgvybiEN9nXiQxIPpIuDBFyxL63PnN7WjHWtqaetzDtjAtacnvBmlCmWflzqG%2FGebULwvSdmsy8Fl0ax%2BkoR0Nd4CQ2jpgCooxNBAp6JlPtWwYAg823OB98OyBxKiRbGchSEwdcDN%2BX%2F3cSEc1OCAIS51RHnzYLv5Z2eavb6G%2BDk3P8cHuxL5AZuxis3mj598CCBlAHkmAoAAFZUnRsmCza4%2F3l83Sy07XZVmJzVwmJqDAA5AU23gaDRUAkM5GIaPAAxAXm3l1AUrdli8EZZf3g4Lg1oRZkxP%2B2ndJzUHPxNQZwfKRrDM74i1spej5ZqSq3o6ElxN%2B8pmtiIhXw4oLoIKABeUd%2Bte8Dnlzypx8BEotS6o1FYley0OdFDS62fNMhAIvW23mzFrKXkexTRYCIRcVxRyP1x%2FEaw9bbGy5ZqOx28ezTYc81W3QjS7k8E7lyD2H%2BpqwgaVy4YzHAdpRAClqsEmqrW0h8eMBVHnGbpJp2iuk5M46CS6u%2B2U4uUx%2FUfhYhjS%2BACKV4Oh2ioqsMTFmbngcwc2LgAJNhge%2BSoeA0tcnJkLZzRLGJNjAYivFRY0lShGw%2FOwoQHIr0Gv7WXfYcCdMAmKn4VC1zVsVAAa6qALJu2ZU4CFaUDZxnRUeFCH1O0o5DyogREj52bEtpAxAjRMXRU%2F9kHSCKnExbm5cEGlYkIuRMWz2VN1X%2BpMbBeEUnsGOUT4HjIigJ362%2Brcb8pKlAtcGjI2AP1z250J35HxpXeJoDg3FBa28NMWrEwb785vdudFEaW0TXgeeCzUh%2BsvtESqyA%2B42PwA2Da1JKKtBKxWT6myQjUoPrZcVNQ2G1BGgBApakkxdWzZKdhf9W%2BaZdQhYSJLT32wOhTaTvfMbCP4GmLn3Ow4yNJXHywl7bNz7AS073VE0fkp8pDFub4ombYFyTQrKarw0ZCFtL4omv76rw%2BC5acsFdWvLMe2ddewdNP1Tf3i9VaK%2Bu3mtDbD0QT7222cMDik1CgLRfujII0HqFA0DDdZED59LBWozkP83j0kN3HYTfinZ74C80dh0gIpAQuoPnS9XnJDlXJkteR2QQtuUqEo5XNcUEDFoet1lv64Xe%2BanEbphko1ZPDgmNh42FcV%2Bp5t45DR9KGKDM%2BCgoUJwdhXE1qDsfMVv98XWWmmajs%2BYXFmLHjGgYwLoCq04uKXH2uebVShxQ6Ormxjy8JS9RicRdU9mVcE0cmmslmCE5LJWDI1oJb062Y%2BZ1mZR96kq6%2Fpu%2FTHztP0rxAeZ8fDcpHhAZSR7vCg0EQZGI6HDAxRFL0MgxXLgjo4Ic%2BhGBDPRwYIUFbaB4R8hyo0jHJbEyo0ALG0jwYlNorAMHVkYAByaQ3Gbdky7i4oyG2oo8M2kNEB9TWt6LhjebrclGkv4aEMD9dEhgfU2XSER29m%2BRSsh1JIM9fwwR3MOiSIyMXIxyW6mhoguobrzcc0iMi5qKLC1HFJrqYGSK4RJbpToGEiU1B1QEGN4ny9DJ66qJUgUQyJjUxH1QEdtYFkF50QJYopcZGJqjogqgqUkDqiiA4fmaKqA4oqy7I0o7hUFROWgUxK1QEpdU5Rx0R0WMj0VB3QU%2BcZ%2B3PDkvDprjz0hRBRjIiDTFTVAVE1Dra3A0poglHGh4dMVdUBVZXzcccWPBT9PP9Qbl7hg0KEqCLE1pAJpjogmHJCHrIg%2FMaNcL1cEBzK4DCQ6aY6oJtWcMSLBaPERR0YFjLV1ABU02SzunkMkoQtSeNQhIWDTCc1AJ20h0Wz9bWD4x88UdldIz7OzoeHTCE1AIWU8%2FF5U6w3xY4SmlYU7aXTkGmkBqCRCnx8qZ4kPlTstUSmlxqAXvrnJkiK%2BK%2BAluAmAMRGJpkagGTaB4TmFmVouMikUgOQSjNavJ8KDx%2BZUmoASmmWxsSFWi5cHZk%2BagD6aB6s1ktao50CDxOZQmoACukODwo3lIFhI1NITUAh3YFBOxnQAeQi01JNQEvNw4yxhHbTqafDR6aktocbinTQbroJWk%2FpyIRUExBSc1YU3AD5%2B3cdE%2Fe9SwTFuaGwkKmnJqCe5t%2FidTWf5PfLOOodCvX77jGhcXY0HGS6qQnopsWuBCjN4uJxRcmuYko8ZBKqCUioRV0LRGyoZcPXkOmnpitYmY6VO7%2FZjRfJo7KOlTNN8GyoX%2F%2F1gY5WUOQHTGxzBNTdNIr6vU2JDUVs2MjmCAvQRMNHFn4bbKJec0q%2Bxsu4eBJAoR77ckBxkS26WYD2GWYsKNj7LF2%2FT78npSb%2Bph9jvi0HtKdyNY%2Fb08UGUrq296Co33mYsmBF%2FX7i7Nyc%2BchW7yxARa05ux%2FpIm%2FGYO0e72eIEJLQzV1HttBnAVJrnPDEojpSvZzPuDH5wFLUo4oQE9lKngXorjtCHrjFCQ51h0EgW8izAOV1mQbRzSYrv%2F59UVYLEB6q8HCRLeRZouRanoGa%2FPIjFo%2BvpPRIDhW6hmwlzwKKVfNgy8hpTILH%2BODCyfGAalZZsVm3%2BTKH4%2Ffr3UGGBIoiUMaHGU4OCiDWbtYR9x7%2F3O3a%2FZDM0zcjHeWQhkL0SKJnfPTh1PTYgJxb07PTcwkcBOAgOxnRtAF5twbnflchTeRMT46B7OxE0xYFW54OBZuIwzIXCAg32ZaVn9edrn6dZZVdojhYpUn08Bgn7VPNweu61V64jctfrrIWS6L2nQ0tjOdf%2F24%2Bufz7f8pXXrnVa%2Fk3%2FXf3Rv6gfs6o6xA2SVT9ShpouWbg8nSThezAQLSSEv89S6l5%2FwubRIFFC3bQ4hlb8lBwywa%2FBmS%2F6q18MIKn3gsamnef%2FFt54aI7Ad7SvUbmaLuzWs3P2NFQf%2BaOje6X%2BxlcRG2W4xIGq6lw0a48o0eMPuDlSvMHyEzBS9uRdGJgTKutJ1IMjCjVcmDKJs6TETN0MMaQGHtyYNpVromBMbrjlhUDI8q3HJg42OLgxTEOzUjOJMDYKIDRbVebBBhI0D2ESRTkjzAzreV3LKTrKtjkV1p4jMHkY%2B6feyxXliux3WPnHhxgaPoQDMPzDRVgQAdgzRcn%2BJBTeRjGrp5zwFWY%2BhRAWDiCEUN3%2FUmIgM6yqo5Ek8tFE1bs8ROOLD%2FhOK8sRnX8aZIauCmrbCYOJC5DX6G5Z4TCP9ZXYAkrXGsSKBxAiy3iVbmBb4os90CSKy1lORoVDwkqpqNPggpUXCvbeehHOo9zimS69toCDaNtUqY20HBEVVU6EYdCDE%2BWi9DbBqOvhQhTd41JiICEU0VZ6kHBy5ImkertYviryVPNUZ5qNInV7dHvUAQTUP%2BqMrN1hssyTSQ7LUNY%2FItlnQiR%2BA5FEAF1snUyPEWEOwLMUhTiHs9Xm0xPzdc4Rx7zJYsWoFPBNo5YWp1rM1FWNJTb0CFj41gr1g3XmQQZQH9tmltMT4tvo8PFR4KLbXmT4AKIs9%2BDLZun2WoSXg5lVAhw6ZJxZbwMKo3OYnKo1%2BuahUWWLrJgEquPyo8Qmr3NuKcORIREWo2bcEVd9jAn51zu1Q%2Bs95qDRPpSu9LOucSnt2WBR9SP4EinTd%2BapOLIBdTYYBPF6XQC3FCRrTySei5wRBdWtwirmApRkVXmNg6mtp60OcXQ7CPRQKOdOJMoIS7QcECJx3Dd%2FdOJIa186HgukLgMxzu8iPP8O1Q5GagfbNmzb5L0xTgQrCBwOt3MNTVe4xlJldeBOsSmE8EyVNIO1USfO7A9Hhgsga2ltfOlYmAmLHQdZsWj5R9Xnjcxj06LUaChG649CRpQqauCwsaRWjIseN0p8hNigSSy9Q33IBfPv0NVCAOosApqIfdXM105CLwLklpIQ3umnOn5d6jCCGpaO60Ic%2BVa0kBqd9G%2BkozKHtetKKLCg9rVSgbC8fYT4cnLsd3XVhdneUMkdFN7rm7Fm0St8fbpu%2FeMU5OdAJOs4hVVW0qPh6xzUNgcz5gyWcyI6m%2FDTJExDAuNjoOPGQ1H0GPZbZ6rmJl9qnDZY3QSNzPKwlVVsJyATBtQTx0n2%2B0koRgZQP%2FdxvkmQIKMgw%2BZrvBhamQ0ZxovA6jA%2Bf5DBP9r6566ZkrPEtMe1Imw7Ik%2FzNK06L88C9aPn9KIla%2F4fw%3D%3D%3C%2Fdiagram%3E%3C%2Fmxfile%3E).


## Build

Before building, the following dependencies must be installed on your system:

 - [jack audio](https://jackaudio.org/)
 - [opencv](https://opencv.org/)
 - [fftw3](https://fftw.org/)
 - [samplerate](https://github.com/libsndfile/libsamplerate)
 - [asound2](https://www.alsa-project.org/wiki/Main_Page)
 - [libx11](https://github.com/mirror/libX11)
 - [xrandr](https://www.x.org/wiki/libraries/libxrandr/)
 - [qt6](https://github.com/qt/qtbase)
 - [sqlite3](https://www.sqlite.org/index.html)
 - [v4l](https://git.linuxtv.org/v4l-utils.git/)
    
To compile the software, follow these steps:
1. Navigate to the project directory:
```bash
cd interactive-audio-visualizer
```
2. Create and enter a build directory:
```bash
mkdir build && cd build
```
3. Generate the build files:
```bash
cmake ..
```
4. Compile the project:
```bash
cmake --build .
```

### Additional Configuration Options

You can customize the build process with the following CMake options:

##### Tests

- To build the tests:
```bash
cmake -DBUILD_TESTS=ON ..
```
- To run the tests:
```bash
./test/test_all
```

##### Static analysis

- To enable static analysis using cppcheck and clang-tidy:
```bash
cmake -DSTATIC_ANALYSIS=ON ..
```
Otherwise, to conduct analytic checks run:
```bash
bash scripts/genStaticAnalysisLogs.sh
```
to generate the `checks/cppcheck.log` and `checks/clang-tidy.log` files within the `checks` directory.

## Docker

To build the Docker image, execute the following command in the project directory:

1. Build the Docker image:

```bash
cd interactive-audio-visualizer
docker build -t iav:latest .
```

2. Run the Docker container:

```bash
xhost +local:docker
docker run --rm --privileged --security-opt seccomp=unconfined -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --device /dev/snd -v /dev:/dev -v $(pwd)/data:/home/iav/interactive-audio-visualizer/data iav:latest
xhost -local:docker
```

## Usage

### Prerequisites

Before running the Interactive Audio Visualizer application, ensure that the following hardware components are connected to your PC:
 - a webcam
 - an audio output device (e.g., speakers or headphones)
 - a display screen (obviously!)

The application will automatically detect available hardware before opening the settings menu.
The application will automatically detect available hardware components before launching the settings menu, upon running the compiled executable.

### Launching the Application

To launch the Interactive Audio Visualizer, run the compiled executable:

```bash
./interactive-audio-visualizer
```

### Configuring Settings

Upon launch, you'll be presented with the settings interface window which allows you to configure various hardware-related settings:

![Interactive Audio Visualizer Settings Interface](files/imgs/iav-settings.png)

#### Audio Settings

- Configure the audio output device
- Adjust sample rate
- Set buffer size
- Modify bit quantization factor

#### Camera Settings

- Select webcam device
- Adjust resolution
- Set frame rate

#### Display Settings

- Configure screen resolution
- Set display frame rate

#### IAV Settings

- Frequency Range: Adjust the frequency range for audio generation.
- ROI (Region of Interest): Configure the area of interest for the video tracking.
- Trigger Mode (Currently only default option is available): Select the trigger mode for initiating the audio visualization (/ visual audiolization) pipeline.
- Tracking Algorithm: Choose a tracking algorithm.
- Accuracy / Economy Slider (Currently unavailable): Adjust performance settings 

### Starting the Experience

Once you have configured the settings, press the **Start button** to begin the interactive audio-visual experience..
Otherwise, pressing **`X` button** in the top-right corner of the interface will close the application.


### Exiting the Application
To close the application, click the **"q" button**.

### Application Workflow

1. A 5-second countdown timer initiates the experience.
2. The webcam captures a frame to initialize the tracking algorithm.
3. The interactive experience begins:
    - The tracking algorithm continuously updates the audiolizer.
    - The audiolizer streams audio and shares audio data with the visualizer.
    - The visualizer updates in real-time, responding to both visual and audio inputs.
4. After 10 seconds, the cycle restarts from the beginning.
5. If the tracking algorithm fails, the application automatically restarts the cycle.'

## Version history

2nd Feb 2025
- [x] Refactor the code to improve readability and maintainability.
- [x] Encapsulate jack audio server in a class, instead of using it through a system call.
- [x] Use Qt for the graphical user interface (GUI).
- [x] Use various libraries (libX11, libxrandr) to detect hardware components (webcam, audio output device, etc.).
- [x] Use SQLite for storing and retrieving settings.
- [x] Make config a thread-safe singleton pattern for configuring the application.
- [x] Thread refactoring to improve performance and resource usage.
- [x] Impletent testing
- [x] Github actions for CI/CD
- [x] Static analysis with cppcheck and clang-tidy
- [x] Add Docker support 
- [x] Enrich visualization with spectrogram representation
- [x] Enrich visualization with audio volume levels visualization
- [x] fix bugs and optimizations
  - [x] fix camera mirroring
  - [x] upgrade compiling to CMake
  - [x] comply to the rule of 5

8th Feb 2024
- [x] instructions update and improvement in compilation method

20th Nov 2023 
- [x] demo app 0.9

## Future work

In the future, the app could evolve to focus on more specific and impactful goals, such as aiding music therapy, assisting those with limited mobility, or supporting individuals with little musical background.

## Feedback

If exciting ideas pop up, like ways to help with music therapy, support people with limited mobility, or assist those with little musical background, I encourage you to fork the project and build upon the existing codebase. Additionaly, feel free to share your insights, improvements, or inspiration on [Gitter](https://matrix.to/#/#interactive-audio-visualizer:gitter.im), or contact me directly at `melissaspaschalis@gmail.com`. Contributions and feedback are always welcome!

## Related

If you liked this project, you may also like:

  - [Eye Harp](https://eyeharp.org/) - Playing music with the eyes

-----------------------------------------------------------------------------------------------------------------------------------------------

<div align="center">
<br>
<br>
<br>
<a href="https://github.com/pasquale90/interactive-audio-visualizer"><img src="files/imgs/thanks.svg" width="1200"></a>

