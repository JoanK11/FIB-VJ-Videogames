using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class HealthBarText : MonoBehaviour
{
    public TMP_Text healthText;
    public Slider slider;

    void Start()
    {
        healthText.text = slider.value.ToString();
    }

    void Update()
    {
        healthText.text = slider.value.ToString();
    }
}
